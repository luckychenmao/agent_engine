#pragma once

#include "engine/worker_base.h"
#include "util/log.h"

namespace engine {

class AdminWorker : public WorkerBase {
public:
    AdminWorker();
    ~AdminWorker();
    AdminWorker(const AdminWorker &) = delete;
    AdminWorker &operator=(const AdminWorker &) = delete;

public:
private:
    LOG_DECLARE();
};

} // namespace engine
