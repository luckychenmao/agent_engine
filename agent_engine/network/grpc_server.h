#pragma once

#include <grpcpp/grpcpp.h>

#include "network/server_config.h"
#include "util/log.h"
#include "util/thread.h"

namespace network {
class GrpcServer {
    using ServerCompletionQueuePtr = std::shared_ptr<grpc::ServerCompletionQueue>;

public:
    GrpcServer();
    ~GrpcServer();
    GrpcServer(const GrpcServer &) = delete;
    GrpcServer &operator=(const GrpcServer &) = delete;

public:
    bool Init(const ServerDescription &desc);
    void Wait();
    void Stop();

private:
    void WorkLoop(const ServerCompletionQueuePtr &cq);

private:
    ServerDescription desc_;
    grpc::ServerBuilder builder_;
    int32_t listen_port_ = 0;
    std::vector<ServerCompletionQueuePtr> completion_queues_;
    std::vector<util::ThreadPtr> work_threads_;
    std::unique_ptr<grpc::Server> server_;
    std::shared_ptr<grpc::Service> service_;

private:
    static constexpr int32_t MaxMessageSize = 1024 * 1024 * 1024;

private:
    LOG_DECLARE();
};

} // namespace network
