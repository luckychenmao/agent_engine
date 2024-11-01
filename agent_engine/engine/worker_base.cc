#include "engine/worker_base.h"

#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>
#include <unordered_map>

#include "engine/common.h"
#include "util/file_util.h"

using namespace util;

namespace engine {

DECLARE_AND_SETUP_LOGGER(engine, WorkerBase);

namespace {
std::atomic_bool is_stop(false);

class SignalHandlerGuard {
public:
    SignalHandlerGuard() {
        previousHandlers[SIGINT] = std::signal(SIGINT, HandleSignal);
        previousHandlers[SIGTERM] = std::signal(SIGTERM, HandleSignal);
        previousHandlers[SIGUSR1] = std::signal(SIGUSR1, HandleSignal);
        previousHandlers[SIGUSR2] = std::signal(SIGUSR2, HandleSignal);
    }

    ~SignalHandlerGuard() {
        for (const auto &[signal, handler] : previousHandlers) {
            if (handler != SIG_ERR) {
                std::signal(signal, handler);
            }
        }
    }

private:
    static void HandleSignal(int sig) {
        if (!is_stop) {
            fprintf(stdout, "receive signal [%d], stop", sig);
            is_stop = true;
        }
    }

    std::unordered_map<int, sighandler_t> previousHandlers;
};
} // namespace

WorkerBase::WorkerBase() {}
WorkerBase::~WorkerBase() {}

bool WorkerBase::Init(int argc, char **argv) {
    if (!InitOptions(argc, argv)) {
        LOG(WARN, "init options failed.");
        return false;
    }
    if (!InitLog()) {
        LOG(WARN, "init options failed.");
        return false;
    }
    if (!DoInit()) {
        LOG(WARN, "DoInit failed.");
        return false;
    }
    return true;
}

bool WorkerBase::Run() {
    is_stop = false;
    is_stopped_ = false;

    SignalHandlerGuard signalGuard; // Ensures RAII for signals

    while (!is_stop && !IsStopped()) {
        if (!DoStart()) {
            DoStop();
            return false;
        }

        LOG(INFO, "Worker is running...");

        while (!is_stop && !IsStopped()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        DoStop();
        LOG(INFO, "Worker is stopped.");
    }

    return true;
}

void WorkerBase::Stop() { is_stopped_ = true; }

bool WorkerBase::IsStopped() const { return is_stopped_; }

bool WorkerBase::RegisterRpcService() {
    // Register RPC services
    return true;
}

bool WorkerBase::InitOptions(int argc, char **argv) {
    option_parser_.AddOption("-l", "--logConf", "logConf", OptionParser::kOptString, false);
    option_parser_.AddOption("-w", "--workDir", "workDir", OptionParser::kOptString, false);
    option_parser_.AddOption("-t", "--threadNum", "threadNum", kRpcServerThreadNum);
    option_parser_.AddOption("-i", "--ioThreadNum", "ioThreadNum", kRpcServerIoThreadNum);
    option_parser_.AddOption("-q", "--queueSize", "queueSize", kRpcServerQueueSize);
    option_parser_.AddOption("-p", "--port", "port", (int32_t)0);
    option_parser_.AddOption("", "--httpPort", "httpPort", (int32_t)0);

    if (!option_parser_.ParseArgs(argc, argv)) {
        std::cerr << "app option parse failed." << std::endl;
        return false;
    }

    option_parser_.GetOptionValue("logConf", log_config_file_);
    option_parser_.GetOptionValue("workDir", cwd_path_);
    option_parser_.GetOptionValue("threadNum", thread_num_);
    option_parser_.GetOptionValue("ioThreadNum", io_thread_num_);
    option_parser_.GetOptionValue("queueSize", queue_size_);
    option_parser_.GetOptionValue("port", port_);
    option_parser_.GetOptionValue("httpPort", http_port_);

    return true;
}

bool WorkerBase::InitLog() {
    if (log_config_file_.empty()) {
        ROOT_LOG_CONFIG();
        return true;
    }
    if (!FileUtil::IsExist(log_config_file_)) {
        std::cerr << "log config file [" << log_config_file_ << "] doesn't exist." << std::endl;
        return false;
    }
    std::string logConfigContent;
    if (!FileUtil::ReadFile(log_config_file_, logConfigContent)) {
        std::cerr << "log config file [" << log_config_file_ << "] read failed." << std::endl;
        return false;
    }
    try {
        alog::Configurator::configureLoggerFromString(logConfigContent.c_str());
    } catch (std::exception &e) {
        std::cerr << "failed to configure logger. logger config file [" << log_config_file_ << "]" << std::endl;
        return false;
    }
    return true;
}

} // namespace engine
