#pragma once

#include "util/log.h"

namespace grpc {
class CompletionQueue;
}

namespace network {

using CompletionQueuePtr = std::shared_ptr<grpc::CompletionQueue>;

struct CompletionQueueStatus {
public:
    CompletionQueueStatus(CompletionQueuePtr _cq) : stopped(false), cq(std::move(_cq)) {}

public:
    volatile bool stopped;
    std::shared_mutex enqueue_lock;
    CompletionQueuePtr cq;
};

using CompletionQueueStatusPtr = std::shared_ptr<CompletionQueueStatus>;

} // namespace network
