#pragma once

#include "engine/WorkerBase.h"
#include "util/Log.h"

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
