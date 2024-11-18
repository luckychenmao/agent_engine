#include "util/thread_pool.h"

#include <ostream>
#include <unistd.h>
#include <utility>

#include "util/legacy/exception.h"
#include "util/log.h"
#include "util/time_utility.h"
#include "util/workitem.h"
#include "util/workitem_queue.h"

using namespace std;

namespace util {

LOG_SETUP(util, ThreadPoolBase);
LOG_SETUP(util, ThreadPool);

ThreadPoolBase::ThreadPoolBase(const size_t threadNum,
                               const size_t queueSize,
                               bool stopIfHasException,
                               const string &name)
    : stop_if_has_exception_(stopIfHasException)
    , active_thread_count_(0)
    , has_exception_(false)
    , thread_num_(threadNum)
    , queue_size_(queueSize)
    , push_(true)
    , run_(false)
    , thread_name_(name) {
    if (thread_num_ == 0) {
        LOG(WARN, "Thread number is zero, use default value 4");
        thread_num_ = DEFAULT_THREADNUM;
    }
    if (queue_size_ == 0) {
        LOG(WARN, "Queue size is zero, use default value 32");
        queue_size_ = DEFAULT_QUEUESIZE;
    }
}

ThreadPoolBase::~ThreadPoolBase() {}

ThreadPoolBase::ERROR_TYPE ThreadPoolBase::PushTask(Task task, bool isBlocked, bool executeWhenFail) {
    WorkItem *workItem = new util::LambdaWorkItem(std::move(task));
    auto ret = PushWorkItem(workItem, isBlocked);
    if (ret != ERROR_NONE) {
        if (executeWhenFail) {
            workItem->process();
        }
        workItem->destroy();
    }
    return ret;
}

bool ThreadPoolBase::Start(const std::string &name) {
    if (run_) {
        LOG(ERROR, "Thread Pool [%s] already started!", name.c_str());
        return false;
    }
    push_ = true;
    run_ = true;
    if (thread_name_.empty() && !name.empty()) {
        thread_name_ = name;
    }
    if (CreateThreads(thread_name_)) {
        return true;
    } else {
        LOG(ERROR, "Thread Pool [%s] failed!", thread_name_.c_str());
        Stop(STOP_THREAD_ONLY);
        return false;
    }
}

void ThreadPoolBase::ConsumeItem(WorkItem *item) {
    try {
        item->process();
        item->destroy();
    } catch (const std::exception &e) {
        util::legacy::ExceptionBase wrapper(e.what());
        wrapper.Init(__FILE__, __FUNCTION__, __LINE__);
        LOG(ERROR,
            "thread [%s] exception: [%s], stack: [%s]",
            thread_name_.c_str(),
            e.what(),
            wrapper.GetStackTrace().c_str());
        DestroyItemIgnoreException(item);
        {
            ScopedLock lock(cond_);
            if (!has_exception_) {
                has_exception_ = true;
                exception_ = e;
            }
        }
    }
}

void ThreadPoolBase::CheckException() {
    ScopedLock lock(cond_);
    if (StopIfHasException()) {
        throw exception_;
    }
}

void ThreadPoolBase::DestroyItemIgnoreException(WorkItem *item) {
    try {
        if (item) {
            item->destroy();
        }
    } catch (...) {
        LOG(ERROR, "Work item destroy exception ignore");
    }
}

void ThreadPoolBase::DropItemIgnoreException(WorkItem *item) {
    try {
        if (item) {
            item->drop();
        }
    } catch (...) {
        LOG(ERROR, "Work item drop exception igonre");
    }
}

void ThreadPoolBase::WaitQueueEmpty() const {
    while (!StopIfHasException()) {
        if ((size_t)0 == GetItemCount()) {
            break;
        }
        usleep(10000);
    }
}

const size_t ThreadPool::MAX_THREAD_NAME_LEN = 15;

ThreadPool::ThreadPool(const size_t threadNum,
                       const size_t queueSize,
                       bool stopIfHasException,
                       const std::string &name,
                       WorkItemQueueFactoryPtr factory)
    : ThreadPoolBase(threadNum, queueSize, stopIfHasException, name) {
    if (factory == nullptr) {
        queue_.reset(new CircularPoolQueue(queue_size_));
    } else {
        queue_.reset(factory->Create(threadNum, queueSize));
    }
}

ThreadPool::ThreadPool(const size_t threadNum,
                       const size_t queueSize,
                       WorkItemQueueFactoryPtr factory,
                       const std::string &name,
                       bool stopIfHasException)
    : ThreadPool(threadNum, queueSize, stopIfHasException, name, factory) {}

ThreadPool::~ThreadPool() { Stop(STOP_AND_CLEAR_QUEUE_IGNORE_EXCEPTION); }

bool ThreadPool::IsFull() const {
    ScopedLock guard(cond_);
    return queue_->size() >= queue_size_;
}

ThreadPool::ERROR_TYPE ThreadPool::PushWorkItem(WorkItem *item, bool isBlocked) {
    if (!push_) {
        LOG(INFO, "thread pool [%s] has stopped", thread_name_.c_str());
        return ERROR_POOL_HAS_STOP;
    }
    if (!item) {
        LOG(INFO, "thread pool [%s] WorkItem is NULL", thread_name_.c_str());
        return ERROR_POOL_ITEM_IS_NULL;
    }
    if (StopIfHasException()) {
        Stop(STOP_AND_CLEAR_QUEUE);
        LOG(INFO, "thread pool [%s] has stopped", thread_name_.c_str());
        return ERROR_POOL_HAS_STOP;
    }

    ScopedLock lock(cond_);
    if (!isBlocked && queue_->size() >= queue_size_) {
        LOG(INFO,
            "thread pool [%s] is full, queueSize [%lu], active thread [%d]",
            thread_name_.c_str(),
            queue_size_,
            active_thread_count_.load());
        return ERROR_POOL_QUEUE_FULL;
    }

    while (push_ && queue_->size() >= queue_size_) {
        cond_.producerWait();
        if (StopIfHasException()) {
            LOG(INFO, "thread pool [%s] has exception, now has stopped", thread_name_.c_str());
            return ERROR_POOL_HAS_STOP;
        }
    }

    if (!push_) {
        LOG(INFO, "thread pool [%s] has stopped", thread_name_.c_str());
        return ERROR_POOL_HAS_STOP;
    }

    do {
        if (queue_->push(item)) {
            break;
        } else {
            return ERROR_POOL_QUEUE_FULL;
        }
        if (isBlocked == false) {
            break;
        }
        usleep(100);
    } while (true);

    cond_.signalConsumer();
    return ERROR_NONE;
}

size_t ThreadPool::GetItemCount() const {
    ScopedLock guard(cond_);
    return queue_->size();
}

void ThreadPool::Stop(STOP_TYPE stopType) {
    if (stopType != STOP_THREAD_ONLY) {
        ScopedLock lock(cond_);
        push_ = false;
        cond_.broadcastProducer();
    }

    if (stopType == STOP_AFTER_QUEUE_EMPTY) {
        WaitQueueEmpty();
    }

    StopThread();

    if (stopType != STOP_THREAD_ONLY) {
        ClearQueue();
    }

    if (stopType != STOP_AND_CLEAR_QUEUE_IGNORE_EXCEPTION) {
        CheckException();
    }
}

void ThreadPool::ClearQueue() {
    ScopedLock lock(cond_);
    while (queue_ && queue_->size()) {
        auto item = queue_->pop();
        DropItemIgnoreException(item);
    }
    cond_.broadcastProducer();
}

void ThreadPool::WaitFinish() {
    while (!StopIfHasException()) {
        {
            ScopedLock guard(cond_);
            if (queue_->size() == 0 && active_thread_count_ == 0) {
                break;
            }
        }
        usleep(10000);
    }
    if (StopIfHasException()) {
        Stop(STOP_AND_CLEAR_QUEUE);
    }
}

void ThreadPool::WorkerLoop() {
    if (thread_start_hook_) {
        thread_start_hook_();
    }
    while (run_ && !StopIfHasException()) {
        WorkItem *item = NULL;
        {
            ScopedLock lock(cond_);
            while (run_ && !queue_->size()) {
                cond_.consumerWait();
            }
            if (!run_) {
                break;
            }
            item = queue_->pop();
            active_thread_count_++;
            cond_.signalProducer();
        }
        ConsumeItem(item);
        active_thread_count_--;
    }
    if (StopIfHasException()) {
        ScopedLock lock(cond_);
        cond_.broadcastProducer();
    }
    if (thread_stop_hook_) {
        thread_stop_hook_();
    }
}

void ThreadPool::StopThread() {
    if (!run_) {
        return;
    }
    {
        ScopedLock lock(cond_);
        run_ = false;
        cond_.broadcastConsumer();
    }
    threads_.clear();
}

bool ThreadPool::CreateThreads(const std::string &name) {
    std::string threadName = name.substr(0, std::min(name.size(), MAX_THREAD_NAME_LEN));
    size_t num = thread_num_;
    while (num--) {
        ThreadPtr tp = Thread::CreateThread(std::bind(&ThreadPool::WorkerLoop, this), threadName);
        threads_.push_back(tp);
        if (!tp) {
            LOG(ERROR, "Create WorkerLoop thread [%s] fail!", name.c_str());
            return false;
        }
    }
    return true;
}

void ThreadPool::AsyncImpl(const std::function<void()> &fn) {
    auto ec = PushTask(fn);
    if (ec != util::ThreadPool::ERROR_NONE) {
        fn();
    }
}

void ThreadPool::StopTrace() {
    util::legacy::ExceptionBase e;
    e.Init("", "", 0);
    stop_backtrace_ = e.ToString();
    LOG(INFO, "StopThreadPool: %s", stop_backtrace_.c_str());
}

} // namespace util
