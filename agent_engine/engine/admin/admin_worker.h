#pragma once

#include "engine/common/worker_base.h"
#include "util/log.h"

namespace engine {
namespace admin {

class AdminWorker : public common::WorkerBase {
public:
    AdminWorker();
    ~AdminWorker();
    AdminWorker(const AdminWorker &) = delete;
    AdminWorker &operator=(const AdminWorker &) = delete;

public:
private:
    LOG_DECLARE();
};

} // namespace admin
} // namespace engine
