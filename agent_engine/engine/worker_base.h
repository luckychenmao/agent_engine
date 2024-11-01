#pragma once

#include "util/log.h"
#include "util/option_parser.h"

namespace engine {

class WorkerBase {
public:
    WorkerBase();
    ~WorkerBase();
    WorkerBase(const WorkerBase &) = delete;
    WorkerBase &operator=(const WorkerBase &) = delete;

public:
    bool Init(int argc, char **argv);
    bool Run();
    void Stop();
    bool IsStopped() const;
    bool RegisterRpcService();

protected:
    virtual bool InitOptions(int argc, char **argv);
    virtual bool InitLog();
    virtual bool DoInit() { return true; }
    virtual bool DoStart() { return true; }
    virtual bool DoStop() { return true; }

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
