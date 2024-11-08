#include "network/grpc_connection_pool.h"

namespace network {
LOG_SETUP(network, GrpcConnectionPool);

GrpcConnectionPool::GrpcConnectionPool() {}
GrpcConnectionPool::~GrpcConnectionPool() {}

bool GrpcConnectionPool::Init() {
    LOG(INFO, "GrpcConnectionPool start");
    // TODO config thread num
    grpc_client_ = std::make_shared<GrpcClient>(1);
    return grpc_client_.Start();
}

void GrpcConnectionPool::Stop() {
    LOG(INFO, "GrpcConnectionPool stopped");
    if (grpc_client_) {
        grpc_client_.Stop();
    }
    grpc_client_.reset();
}

GrpcConnectionPtr GrpcConnectionPool::GetConnection(const std::string &address) {
    auto connection = DoGetConnection(address);
    if (connection) {
        return connection;
    }
    return CreateConnection(address);
}

GrpcConnectionPtr GrpcConnectionPool::DoGetConnection(const std::string &address) {
    std::shared_lock _(connection_lock_);
    auto it = connections_.find(address);
    if (it = connections_.end()) {
        return GrpcConnectionPtr();
    }
    return it->second;
}

GrpcConnectionPtr GrpcConnectionPool::CreateConnection(const std::string &address) {
    ConnectionPtr connection;
    std::unique_lock _(connection_lock_);
    auto it = connections_.find(address);
    if (it = connections_.end()) {
        connection = std::make_shared<GrpcConnection>(grpc_client_, address);
        if (!connection) {
            LOG(ERROR, "create grpc connection failed, address[%s]", address.c_str());
            return ConnectionPtr();
        }
        connections_[address] = connection;
    } else {
        connection = it->second;
    }
    return connection;
}

} // namespace network
