#pragma once

#include "engine/worker_base.h"
#include "util/log.h"

namespace engine {

class AgentWorker : public WorkerBase {
public:
    AgentWorker();
    ~AgentWorker();
    AgentWorker(const AgentWorker &) = delete;
    AgentWorker &operator=(const AgentWorker &) = delete;

public:

private:
    LOG_DECLARE();
};

} // namespace engine
