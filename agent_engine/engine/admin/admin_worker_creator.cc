#include "engine/admin/admin_worker.h"
#include "engine/common/worker_base.h"

namespace engine {
namespace common {

WorkerBase *CreateWorker() { return new admin::AdminWorker(); }

}; // namespace common
} // namespace engine
