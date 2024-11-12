#pragma once

#include "engine/common/worker_base.h"
#include "util/log.h"

namespace engine {
namespace worker {

class Worker : public common::WorkerBase {
public:
    Worker();
    ~Worker();
    Worker(const Worker &) = delete;
    Worker &operator=(const Worker &) = delete;

private:
    LOG_DECLARE();
};

} // namespace worker
} // namespace engine
