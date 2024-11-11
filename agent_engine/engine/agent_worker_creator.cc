#include "engine/agent_worker.h"
#include "engine/worker_base.h"

namespace engine {

WorkerBase *CreateWorker() { return new AgentWorker(); }

}; // namespace engine
