#include "engine/common/worker_base.h"
#include "engine/worker/worker.h"

namespace engine {
namespace common {

WorkerBase *CreateWorker() { return new worker::Worker(); }

} // namespace common
}; // namespace engine
