#pragma once

#include "util/Log.h"

namespace engine {

class WorkerBase {
public:
    WorkerBase();
    ~WorkerBase();
    WorkerBase(const WorkerBase &) = delete;
    WorkerBase &operator=(const WorkerBase &) = delete;

public:
    bool init(int argc, char **argv);
    bool run();
    void stop();
    bool isStopped() const;
    bool registerRpcService();

protected:
    virtual bool doInit() { return true; }
    virtual bool doStart() { return true; }
    virtual bool doStop() { return true; }

private:
    volatile bool _isStopped = true;
};

WorkerBase *createWorker();

} // namespace engine
