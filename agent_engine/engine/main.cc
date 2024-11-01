#include <memory>

#include "engine/worker_base.h"
#include "util/log.h"
#include "util/scope.h"

using namespace engine;

int32_t main(int argc, char **argv) {
    util::ScopeGuard _([] { LOG_SHUTDOWN(); });

    std::shared_ptr<WorkerBase> worker;
    worker.reset(CreateWorker());
    if (!worker) {
        std::cerr << "create worker failed" << std::endl;
        return -1;
    }
    bool succ = worker->Init(argc, argv);
    if (!succ) {
        std::cerr << "app init failed" << std::endl;
        return -1;
    }
    succ = worker->Run();
    if (!succ) {
        std::cerr << "app start failed" << std::endl;
        return -1;
    }
    return 0;
}
