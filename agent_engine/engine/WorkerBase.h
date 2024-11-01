#pragma once

#include "util/Log.h"
#include "util/OptionParser.h"

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
    virtual bool initOptions(int argc, char **argv);
    virtual bool initLog();
    virtual bool doInit() { return true; }
    virtual bool doStart() { return true; }
    virtual bool doStop() { return true; }

private:
    volatile bool is_stopped_ = true;
    util::OptionParser option_parser_;
    std::string log_config_file_;
    std::string cwd_path_;
    uint32_t thread_num_;
    uint32_t io_thread_num_;
    uint32_t queue_size_;
    int32_t port_;
    int32_t http_port_;
};

WorkerBase *CreateWorker();

} // namespace engine
