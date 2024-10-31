#include "engine/WorkerBase.h"

#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>
#include <unordered_map>

using namespace util;

namespace engine {

DECLARE_AND_SETUP_LOGGER(engine, WorkerBase);

namespace {
std::atomic_bool isStop(false);

class SignalHandlerGuard {
public:
    SignalHandlerGuard() {
        previousHandlers[SIGINT] = std::signal(SIGINT, handleSignal);
        previousHandlers[SIGTERM] = std::signal(SIGTERM, handleSignal);
        previousHandlers[SIGUSR1] = std::signal(SIGUSR1, handleSignal);
        previousHandlers[SIGUSR2] = std::signal(SIGUSR2, handleSignal);
    }

    ~SignalHandlerGuard() {
        for (const auto &[signal, handler] : previousHandlers) {
            if (handler != SIG_ERR) {
                std::signal(signal, handler);
            }
        }
    }

private:
    static void handleSignal(int sig) {
        if (!isStop) {
            fprintf(stdout, "receive signal [%d], stop", sig);
            isStop = true;
        }
    }

    std::unordered_map<int, sighandler_t> previousHandlers;
};
} // namespace

WorkerBase::WorkerBase() {}
WorkerBase::~WorkerBase() {}

bool WorkerBase::init(int argc, char **argv) {
    // Initialize worker
    return true;
}

bool WorkerBase::run() {
    isStop = false;
    _isStopped = false;

    SignalHandlerGuard signalGuard; // Ensures RAII for signals

    while (!isStop && !isStopped()) {
        if (!doStart()) {
            doStop();
            return false;
        }

        LOG(INFO, "Worker is running...");

        while (!isStop && !isStopped()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        doStop();
        LOG(INFO, "Worker is stopped.");
    }

    return true;
}

void WorkerBase::stop() { _isStopped = true; }

bool WorkerBase::isStopped() const {
    return _isStopped;
}

bool WorkerBase::registerRpcService() {
    // Register RPC services
    return true;
}

} // namespace engine
