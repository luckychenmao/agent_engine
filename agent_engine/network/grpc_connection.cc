#include "network/grpc_connection.h"

#include <grpcpp/grpcpp.h>
#include <shared_mutex>

#include "network/grpc_client.h"
#include "engine/proto/agent_service.grpc.pb.h"

namespace network {
LOG_SETUP(network, GrpcConnection);

GrpcConnection::GrpcConnection(const GrpcClientPtr &grpc_client, const std::string &address)
    : id_(GenAllocId()), grpc_client_(grpc_client), address_(address) {}
GrpcConnection::~GrpcConnection() {}

void GrpcConnection::Post(const google::protobuf::Message &_) {
    auto channel = GetChannel();
    if (!channel) {
        LOG(WARN, "get channel failed");
        return;
    }
    auto stub = proto::AgentChatService::NewStub(channel);
    grpc::ClientContext context;
    auto cqs = grpc_client_->GetCompletionQueue(id_);
    auto stream = stub->AsyncChat(&context, cqs->cq.get(), reinterpret_cast<void *>(1));

    proto::ChatRequest request;
    request.set_appid(123);
    request.set_userid(456);
    request.set_content("Hello from client!");

    stream->Write(request, reinterpret_cast<void *>(2));
}

GrpcChannelPtr GrpcConnection::GetChannel() {
    {
        std::shared_lock _(channel_lock_);
        if (channel_) {
            return channel_;
        }
    }
    // 写锁
    std::unique_lock _(channel_lock_);
    if (channel_) {
        return channel_;
    }
    if (!channel_args_) {
        channel_ = grpc::CreateChannel(address_, channel_credentials_);
    } else {
        channel_ = grpc::CreateCustomChannel(address_, channel_credentials_, *channel_args_);
    }
    return channel_;
}

size_t GrpcConnection::GenAllocId() {
    static std::atomic<size_t> count = {0};
    return count.fetch_add(1);
}

} // namespace network
