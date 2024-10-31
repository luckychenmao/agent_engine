#include <memory>

#include "engine/WorkerBase.h"
#include "util/Log.h"
#include "util/Scope.h"

using namespace engine;

int32_t main(int argc, char **argv) {
    util::ScopeGuard _([] { LOG_SHUTDOWN(); });

    std::shared_ptr<WorkerBase> worker;
    worker.reset(createWorker());
    if (!worker) {
        std::cout << "create worker failed" << std::endl;
        return -1;
    }
    bool succ = worker->init(argc, argv);
    if (!succ) {
        std::cout << "app init failed" << std::endl;
        return -1;
    }
    succ = worker->run();
    if (!succ) {
        std::cout << "app start failed" << std::endl;
        return -1;
    }
    return 0;
}
