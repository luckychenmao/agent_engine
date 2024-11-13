#pragma once

#include <iosfwd>
#include <memory>
#include <semaphore.h>
#include <stddef.h>
#include <stdint.h>
#include <string>

#include "util/lock_free_queue.h"
#include "util/thread.h"
#include "util/thread_pool.h"
#include "util/workitem.h"
#include "util/workitem_queue.h"

namespace util {

class ThreadAutoScaler;

class LockFreeThreadPoolQueue : public util::WorkItemQueue {
public:
    LockFreeThreadPoolQueue() : WorkItemQueue(){};

    virtual bool push(util::WorkItem *item) override {
        queue_.Push(item);
        return true;
    }
    virtual util::WorkItem *pop() override {
        util::WorkItem *item;
        if (queue_.Pop(&item))
            return item;
        return NULL;
    }
    virtual size_t size() const override { return queue_.Size(); }

private:
    LockFreeQueue<util::WorkItem *> queue_;
};

class LockFreeThreadPool : public util::ThreadPool {
public:
    LockFreeThreadPool(const size_t threadNum,
                       const size_t queueSize,
                       util::WorkItemQueueFactoryPtr factory,
                       const std::string &name,
                       bool stopIfHasException = false);
    virtual ~LockFreeThreadPool();

private:
    LockFreeThreadPool(const LockFreeThreadPool &);
    LockFreeThreadPool &operator=(const LockFreeThreadPool &);

public:
    virtual ERROR_TYPE PushWorkItem(util::WorkItem *item, bool isBlocked = true) override;
    virtual size_t GetItemCount() const override;
    virtual bool Start(const std::string &name = "") override;
    virtual void Stop(STOP_TYPE stopType = STOP_AFTER_QUEUE_EMPTY) override;
    void Join();

protected:
    virtual void ClearQueue() override;
    void SetIdleThresholdInMs(int ms);
    virtual bool CreateThreads(const std::string &name = "") override;

private:
    bool DoInit();
    bool InitAutoScaler();
    bool IsRunning() const;
    int WorkerRoutine(size_t id);
    virtual void ConsumeItem(util::WorkItem *item) override;

private:
    volatile uint64_t cur_queue_size_;
    util::ThreadCountedCond *notifier_;
    int64_t notify_index_;
    int64_t wait_index_;
    uint64_t notify_slots_;
    ThreadAutoScaler *auto_scaler_;

public:
    static constexpr size_t AUTO_SCALE_THREAD_NUM = (size_t)-1;

private:
    LOG_DECLARE();
};

} // namespace util
