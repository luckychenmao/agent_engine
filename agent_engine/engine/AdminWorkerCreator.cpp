#include "engine/AdminWorker.h"
#include "engine/WorkerBase.h"

namespace engine {

WorkerBase *CreateWorker() { return new AdminWorker(); }

}; // namespace engine
