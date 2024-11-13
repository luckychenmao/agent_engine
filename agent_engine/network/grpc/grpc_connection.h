#pragma once

#include <shared_mutex>

#include "util/log.h"

namespace google {
namespace protobuf {
class Message;
}
} // namespace google

namespace grpc {
class ChannelCredentials;
class ChannelArguments;
class ChannelInterface;
} // namespace grpc

namespace network {
class GrpcClient;
using GrpcClientPtr = std::shared_ptr<GrpcClient>;
using GrpcChannelPtr = std::shared_ptr<grpc::ChannelInterface>;
class GrpcConnection {
public:
    GrpcConnection(const GrpcClientPtr &grpc_client, const std::string &address);
    ~GrpcConnection();
    GrpcConnection(const GrpcConnection &) = delete;
    GrpcConnection &operator=(const GrpcConnection &) = delete;

public:
    void Post(const google::protobuf::Message &request);

private:
    GrpcChannelPtr GetChannel();
    static size_t GenAllocId();

private:
    size_t id_;
    GrpcClientPtr grpc_client_;
    std::string address_;
    std::shared_mutex channel_lock_;
    GrpcChannelPtr channel_;
    std::shared_ptr<grpc::ChannelCredentials> channel_credentials_;
    std::shared_ptr<grpc::ChannelArguments> channel_args_;

private:
    LOG_DECLARE();
};

using GrpcConnectionPtr = std::shared_ptr<GrpcConnection>;

} // namespace network
