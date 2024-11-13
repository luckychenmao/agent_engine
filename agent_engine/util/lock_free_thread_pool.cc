#include "util/lock_free_thread_pool.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <errno.h>
#include <exception>
#include <functional>
#include <memory>
#include <new>
#include <stack>
#include <string>
#include <typeinfo>
#include <unistd.h>

#include "util/lock.h"
#include "util/lock_free.h"
#include "util/log.h"
#include "util/thread.h"
#include "util/thread_auto_scaler.h"
#include "util/time_utility.h"
#include "util/workitem.h"

namespace util {

LOG_SETUP(util, LockFreeThreadPool);

LockFreeThreadPool::LockFreeThreadPool(const size_t threadNum,
                                       const size_t queueSize,
                                       util::WorkItemQueueFactoryPtr factory,
                                       const std::string &name,
                                       bool stopIfHasException)
    : util::ThreadPool(threadNum, queueSize, stopIfHasException, name, factory)
    , cur_queue_size_(0)
    , notifier_(nullptr)
    , notify_index_(0)
    , wait_index_(0)
    , notify_slots_(threadNum)
    , auto_scaler_(nullptr) {
    if (factory == NULL) {
        queue_.reset(new (std::nothrow) LockFreeThreadPoolQueue());
    } else {
        queue_.reset(factory->Create(threadNum, queueSize));
    }
}

LockFreeThreadPool::~LockFreeThreadPool() {
    Stop();
    util::WorkItem *item = NULL;
    if (queue_.get()) {
        while ((item = queue_->pop())) {
            item->destroy();
        }
    }
    delete[] notifier_;
    delete auto_scaler_;
    notifier_ = nullptr;
    auto_scaler_ = nullptr;
}

bool LockFreeThreadPool::InitAutoScaler() {
    if (AUTO_SCALE_THREAD_NUM == thread_num_) {
        std::unique_ptr<ThreadAutoScaler> scaler(new ThreadAutoScaler());
        if (scaler->Init(thread_name_)) {
            auto_scaler_ = scaler.release();
            return true;
        } else {
            LOG(ERROR, "thread num auto scale init failed");
            return false;
        }
    }
    return true;
}

bool LockFreeThreadPool::DoInit() {
    if (!InitAutoScaler()) {
        return false;
    }
    if (auto_scaler_) {
        thread_num_ = auto_scaler_->GetMaxThreadNum();
        LOG(INFO, "auto scaler enabled, thread num min[%lu], max[%lu]", auto_scaler_->GetMinThreadNum(), thread_num_);
    }
    notify_slots_ = thread_num_;
    notifier_ = new (std::nothrow) util::ThreadCountedCond[notify_slots_];
    if (notifier_ == NULL)
        return false;
    return true;
}

bool LockFreeThreadPool::Start(const std::string &name) {
    if (!DoInit()) {
        return false;
    }
    util::ScopedLock lock(cond_);
    return ThreadPool::Start(name);
}

void LockFreeThreadPool::Stop(STOP_TYPE stopType) {
    {
        util::ScopedLock guard(cond_);
        if (!run_) {
            return;
        }
        run_ = false;
        push_ = false;
    }
    if (auto_scaler_) {
        auto_scaler_->Stop();
    }
    Join();
}

LockFreeThreadPool::ERROR_TYPE LockFreeThreadPool::PushWorkItem(util::WorkItem *item, bool isBlocked) {
    if (!IsRunning()) {
        return ERROR_POOL_HAS_STOP;
    }

    if (__builtin_expect(cur_queue_size_ > queue_size_, 0)) {
        if (isBlocked) {
            while (cur_queue_size_ > queue_size_)
                usleep(100);
        } else {
            return ERROR_POOL_QUEUE_FULL;
        }
    }

    if (item) {
        AtomicIncrement(&cur_queue_size_);
        do {
            if (queue_->push(item)) {
                item = NULL;
                break;
            }
            if (isBlocked == false) {
                break;
            }
            usleep(100);
        } while (1);

        if (item) { // push failure
            AtomicDecrement(&cur_queue_size_);
            return ERROR_POOL_QUEUE_FULL;
        }
        size_t index = AtomicIncrement(&notify_index_) % notify_slots_;
        notifier_[index].signalCounted();
    }

    return ERROR_NONE;
}

size_t LockFreeThreadPool::GetItemCount() const { return cur_queue_size_; }

bool LockFreeThreadPool::IsRunning() const { return run_; }

int LockFreeThreadPool::WorkerRoutine(size_t id) {
    while (true) {
        if (auto_scaler_) {
            auto_scaler_->Wait(id);
        }
        size_t index = AtomicIncrement(&wait_index_) % notify_slots_;
        notifier_[index].waitCounted();

        if (!run_) {
            break;
        }

        util::WorkItem *item;
        while (!(item = queue_->pop())) {
#if __x86_64__
            asm("pause");
#elif __aarch64__
#define __nops(n) ".rept " #n "\nnop\n.endr\n"
            asm volatile(__nops(1));
#endif
        }
        ++active_thread_count_;
        ConsumeItem(item);
        --active_thread_count_;
    }

    return 0;
}

void LockFreeThreadPool::ConsumeItem(util::WorkItem *item) {
    int64_t end, start;
    AtomicDecrement(&cur_queue_size_);
    start = util::TimeUtility::MonotonicTimeUs();
    ThreadPoolBase::ConsumeItem(item);
    end = util::TimeUtility::MonotonicTimeUs();
    if (end - start >= 2000000) {
        LOG(WARN, "process cost: %ld, cur_queue_size_: %ld", end - start, cur_queue_size_);
    }
}

bool LockFreeThreadPool::CreateThreads(const std::string &name) {
    size_t num = thread_num_;
    threads_.resize(num, NULL);
    while (num--) {
        threads_[num] =
            util::Thread::CreateThread(std::bind(&LockFreeThreadPool::WorkerRoutine, this, num), thread_name_);
        if (!threads_[num]) {
            LOG(ERROR, "Create WorkerLoop thread fail!");
            return false;
        }
    }
    return true;
}

void LockFreeThreadPool::Join() {
    for (size_t i = 0; i < notify_slots_; ++i) {
        notifier_[i].signalCounted();
    }

    for (size_t i = 0; i < thread_num_; i++) {
        if (threads_[i]) {
            threads_[i]->Join();
            threads_[i].reset();
        }
    }
}

void LockFreeThreadPool::ClearQueue() {}

} // namespace util
