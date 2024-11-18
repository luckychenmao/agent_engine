#pragma once

#include "engine/common/base_module.h"
#include "engine/proto/agent_service.grpc.pb.h"
#include "util/log.h"

namespace network {
class GrpcServer;
}

namespace engine {
namespace worker {

class AgentServiceModule : public common::BaseModule, public proto::AgentChatService::AsyncService {
public:
    AgentServiceModule();
    ~AgentServiceModule();
    AgentServiceModule(const AgentServiceModule &) = delete;
    AgentServiceModule &operator=(const AgentServiceModule &) = delete;

public:
    bool DoInit() override;
    bool DoLoad() override;
    bool DoUnload() override;

public:
    GRPC_METHOD_HELPER(Chat, proto::ChatRequest, proto::ChatResponse);

private:
    std::unique_ptr<network::GrpcServer> grpc_server_;

private:
    LOG_DECLARE();
};
} // namespace worker
} // namespace engine
