#include "engine/AdminWorker.h"
#include "engine/WorkerBase.h"

namespace engine {

WorkerBase *createWorker() { return new AdminWorker(); }

}; // namespace engine
