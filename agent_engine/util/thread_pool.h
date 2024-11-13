/*
 * Copyright 2014-present Alibaba Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <future>
#include <iosfwd>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "util/lock.h"
#include "util/log.h"
#include "util/thread.h"
#include "util/workitem.h"
#include "util/workitem_queue.h"

namespace util {

class ThreadPoolBase {
public:
    using Task = std::function<void()>;

    template <typename T = void>
    using Future = std::future<T>;

    enum {
        DEFAULT_THREADNUM = 4,
        DEFAULT_QUEUESIZE = 32,
    };

    enum STOP_TYPE {
        STOP_THREAD_ONLY,
        STOP_AND_CLEAR_QUEUE,
        STOP_AFTER_QUEUE_EMPTY,
        STOP_AND_CLEAR_QUEUE_IGNORE_EXCEPTION,
    };

    enum ERROR_TYPE {
        ERROR_NONE = 0,
        ERROR_POOL_HAS_STOP,
        ERROR_POOL_ITEM_IS_NULL,
        ERROR_POOL_QUEUE_FULL,
    };

public:
    ThreadPoolBase(const size_t threadNum = DEFAULT_THREADNUM,
                   const size_t queueSize = DEFAULT_QUEUESIZE,
                   bool stopIfHasException = false,
                   const std::string &name = "");
    virtual ~ThreadPoolBase();
    ThreadPoolBase(const ThreadPoolBase &) = delete;
    ThreadPoolBase &operator=(const ThreadPoolBase &) = delete;

public:
    virtual bool IsFull() const = 0;
    virtual ERROR_TYPE PushWorkItem(WorkItem *item, bool isBlocked = true) = 0;
    virtual size_t GetItemCount() const = 0;
    virtual bool Start(const std::string &name = "");
    virtual void Stop(STOP_TYPE stopType = STOP_AFTER_QUEUE_EMPTY) = 0;

public:
    ERROR_TYPE PushTask(Task task, bool isBlocked = true, bool executeWhenFail = false);

    template <typename Lambda, typename Ret = std::invoke_result_t<Lambda>>
    Future<Ret> Async(Lambda &&lambda);

    template <typename Lambda,
              typename = std::enable_if_t<std::is_same_v<bool, std::invoke_result_t<Lambda, int>>, void>>
    bool BlockingParallel(int parallel_count, Lambda &&lambda);

public:
    inline const std::string &GetName() const { return thread_name_; }
    inline size_t GetActiveThreadNum() const { return active_thread_count_; };
    inline size_t GetQueueSize() const { return queue_size_; };
    inline size_t GetThreadNum() const { return thread_num_; };
    virtual void ConsumeItem(WorkItem *);
    void CheckException();
    static void DestroyItemIgnoreException(WorkItem *item);
    static void DropItemIgnoreException(WorkItem *item);

protected:
    virtual void ClearQueue() = 0;
    virtual void WaitQueueEmpty() const;
    virtual bool CreateThreads(const std::string &name) = 0;
    virtual void AsyncImpl(const std::function<void()> &) = 0;
    inline bool StopIfHasException() const { return has_exception_ && stop_if_has_exception_; }
    bool stop_if_has_exception_;
    std::atomic<std::uint32_t> active_thread_count_;
    std::atomic_bool has_exception_;
    std::exception exception_;
    size_t thread_num_;
    size_t queue_size_;
    mutable ProducerConsumerCond cond_;
    std::atomic_bool push_;
    std::atomic_bool run_;
    std::string thread_name_;

private:
    friend class ThreadPoolTest;

    LOG_DECLARE();
};

template <typename Lambda, typename Ret>
ThreadPoolBase::Future<Ret> ThreadPoolBase::Async(Lambda &&lambda) {
    auto task = std::make_shared<std::packaged_task<Ret()>>(std::forward<Lambda>(lambda));
    auto future = task->get_future();
    asyncImpl([task = std::move(task)] { (*task)(); });
    return future;
}

template <typename Lambda, typename U>
bool ThreadPoolBase::BlockingParallel(int parallel_count, Lambda &&lambda) {
    std::vector<Future<bool>> futures;
    futures.reserve(parallel_count - 1);
    for (int i = 1; i < parallel_count; ++i) {
        futures.emplace_back(async([&, i]() -> bool { return lambda(i); }));
    }
    bool result = lambda(0);
    for (auto &future : futures) {
        future.wait();
        assert(future.valid());
        auto r = future.get();
        if (!r) {
            result = false;
        }
    }
    return result;
}

class ThreadPool : public ThreadPoolBase {
    static const size_t MAX_THREAD_NAME_LEN;

public:
    using ThreadHook = std::function<void()>;
    ThreadPool(const size_t threadNum = DEFAULT_THREADNUM,
               const size_t queueSize = DEFAULT_QUEUESIZE,
               bool stopIfHasException = false,
               const std::string &name = "",
               WorkItemQueueFactoryPtr factory = NULL);
    ThreadPool(const size_t threadNum,
               const size_t queueSize,
               WorkItemQueueFactoryPtr factory,
               const std::string &name,
               bool stopIfHasException = false);
    virtual ~ThreadPool();
    virtual bool IsFull() const override;
    virtual ERROR_TYPE PushWorkItem(WorkItem *item, bool isBlocked = true) override;
    virtual size_t GetItemCount() const override;
    virtual void Stop(STOP_TYPE stopType = STOP_AFTER_QUEUE_EMPTY) override;
    virtual void WaitFinish();
    const std::vector<ThreadPtr> &GetThreads() const { return threads_; }
    void SetThreadStartHook(const ThreadHook &startHook) { thread_start_hook_ = startHook; }
    void SetThreadStopHook(const ThreadHook &stopHook) { thread_stop_hook_ = stopHook; }

protected:
    virtual void ClearQueue() override;
    virtual void WorkerLoop();
    virtual void StopThread();
    virtual bool CreateThreads(const std::string &name) override;
    virtual void AsyncImpl(const std::function<void()> &) override;
    void StopTrace();
    ThreadHook thread_start_hook_;
    ThreadHook thread_stop_hook_;
    std::unique_ptr<WorkItemQueue> queue_;
    std::vector<ThreadPtr> threads_;
    std::string stop_backtrace_;

private:
    LOG_DECLARE();
};

typedef std::shared_ptr<ThreadPool> ThreadPoolPtr;
typedef std::shared_ptr<ThreadPoolBase> ThreadPoolBasePtr;

} // namespace util
