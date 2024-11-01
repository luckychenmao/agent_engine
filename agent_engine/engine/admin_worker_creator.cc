#include "engine/admin_worker.h"
#include "engine/worker_base.h"

namespace engine {

WorkerBase *CreateWorker() { return new AdminWorker(); }

}; // namespace engine
