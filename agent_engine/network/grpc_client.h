#pragma once

#include "util/log.h"

namespace grpc {
class Alarm;
}
namespace network {

class GrpcClient {
public:
    GrpcClient(size_t thread_num);
    ~GrpcClient();
    GrpcClient(const GrpcClient &) = delete;
    GrpcClient &operator=(const GrpcClient &) = delete;

public:
    bool start();
    void stop();
    const CompletionQueueStatusPtr &GetCompletionQueue(size_t id);

private:
    void WorkLoop(size_t idx, CompletionQueueStatusPtr cqsPtr);

private:
    size_t thread_num_;
    std::vector<std::unique_ptr<grpc::Alarm>> shutdown_alarms_;
    std::vector<util::ThreadPtr> work_threads_;
    std::vector<CompletionQueueStatusPtr> completion_queues_;

private:
    LOG_DECLARE();
};

} // namespace network
