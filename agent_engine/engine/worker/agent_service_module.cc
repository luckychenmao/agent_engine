#include "engine/modules/agent_service_module.h"

#include "engine/common.h"
#include "engine/creator_factory.h"
#include "network/grpc_server.h"
#include "network/server_config.h"

namespace engine {
namespace worker {

LOG_SETUP(engine.modules, AgentServiceModule);

AgentServiceModule::AgentServiceModule() {}
AgentServiceModule::~AgentServiceModule() {}

bool AgentServiceModule::DoInit() {
    grpc_server_ = std::make_unique<network::GrpcServer>();
    network::ServerDescription desc;
    return grpc_server_->Init(desc);
}
bool AgentServiceModule::DoLoad() {
    auto succ = grpc_server_->RegisterService(this);
    if (!succ) {
        LOG(ERROR, "register agent service failed");
        return false;
    }
    succ = grpc_server_->Start();
    if (!succ) {
        LOG(ERROR, "start agent service failed");
        return false;
    }
    return true;
}

bool AgentServiceModule::DoUnload() {
    grpc_server_->Stop();
    return true;
}

REGISTER_MODULE(AgentServiceModule);

} // namespace worker
} // namespace engine
