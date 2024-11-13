#pragma once

#include <deque>

#include "util/circular_queue.h"
#include "util/workitem.h"

namespace util {

class WorkItemQueue {
public:
    WorkItemQueue(){};
    virtual ~WorkItemQueue(){};
    virtual bool push(WorkItem *item) = 0;
    virtual WorkItem *pop() = 0;
    virtual size_t size() const = 0;
};

class WorkItemQueueFactory {
public:
    virtual ~WorkItemQueueFactory(){};
    virtual WorkItemQueue *Create(const size_t threadNum, const size_t queueSize) = 0;
};
typedef std::shared_ptr<WorkItemQueueFactory> WorkItemQueueFactoryPtr;

class CircularPoolQueue : public WorkItemQueue {
public:
    CircularPoolQueue(const size_t queueSize) : queue_(queueSize){};
    virtual bool push(WorkItem *item) override {
        if (queue_.size() >= queue_.capacity()) {
            return false;
        }
        queue_.push_back(item);
        return true;
    }
    virtual WorkItem *pop() override {
        if (queue_.empty()) {
            return nullptr;
        }
        WorkItem *item = queue_.front();
        queue_.pop_front();
        return item;
    }
    virtual size_t size() const override { return queue_.size(); }

private:
    CircularQueue<WorkItem *> queue_;
};

class ThreadPoolQueue : public WorkItemQueue {
public:
    ThreadPoolQueue(){};
    virtual bool push(WorkItem *item) override {
        queue_.push_back(item);
        return true;
    }
    virtual WorkItem *pop() override {
        if (queue_.empty()) {
            return nullptr;
        }
        WorkItem *item = queue_.front();
        queue_.pop_front();
        return item;
    }
    virtual size_t size() const override { return queue_.size(); }

private:
    std::deque<WorkItem *> queue_;
};

class ThreadPoolQueueFactory : public WorkItemQueueFactory {
public:
    WorkItemQueue *Create(const size_t threadNum, const size_t queueSize) override { return new ThreadPoolQueue(); }
};

} // namespace util
