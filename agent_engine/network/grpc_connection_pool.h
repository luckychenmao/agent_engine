#pragma once

#include "util/log.h"

namespace network {
class GrpcClient;
class GrpcConnection;
class GrpcConnectionPool {
    using GrpcConnectionPtr = std::shared_ptr<GrpcConnection>;
    using GrpcClientPtr = std::shared_ptr<GrpcClient>;
    using GrpcConnectionMap = std::unordered_map<std::string, GrpcConnectionPtr>;

public:
    GrpcConnectionPool();
    ~GrpcConnectionPool();
    GrpcConnectionPool(const GrpcConnectionPool &) = delete;
    GrpcConnectionPool &operator=(const GrpcConnectionPool &) = delete;

public:
    bool Init();
    void Stop();
    GrpcConnectionPtr GetConnection(const std::string &address);

private:
    GrpcConnectionPtr DoGetConnection(const std::string &address);
    GrpcConnectionPtr CreateConnection(const std::string &address);

private:
    GrpcClientPtr grpc_client_;
    std::shared_mutex connection_lock_;
    GrpcConnectionMap connections_;

private:
    LOG_DECLARE();
};

} // namespace network
