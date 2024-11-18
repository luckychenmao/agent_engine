#pragma once

#include <string>

#include "agent_engine/util/thread_pool.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"

ARPC_BEGIN_NAMESPACE(arpc)

/**
 * @ingroup ServerClasses
 * Descriptor of a thread pool.
 * Each thread pool will be defined with a pool name, thread
 * number that the pool can hold and the work item queue limit.
 */
class ThreadPoolDescriptor {
public:
    ThreadPoolDescriptor() : threadPoolName(DEFAULT_TREAHDPOOL_NAME) {
        threadNum = 0;
        queueSize = 0;
        _allowSharedPoolOverride = true;
    }

    ThreadPoolDescriptor(std::string thPoolName, size_t threadNum, size_t queueSize) : threadPoolName(thPoolName) {
        this->threadNum = threadNum;
        this->queueSize = queueSize;
    }

    ThreadPoolDescriptor(std::string thPoolName,
                         size_t threadNum,
                         size_t queueSize,
                         util::WorkItemQueueFactoryPtr factory)
        : threadPoolName(thPoolName) {
        this->threadNum = threadNum;
        this->queueSize = queueSize;
        this->factory = factory;
    }

    void allowSharedPoolOverride() { _allowSharedPoolOverride = true; }

    std::string threadPoolName;
    size_t threadNum;
    size_t queueSize;
    util::WorkItemQueueFactoryPtr factory;
    bool _allowSharedPoolOverride = false;
};

ARPC_END_NAMESPACE(arpc)