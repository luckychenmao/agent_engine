#pragma once

#include <functional>
#include <memory>
#include <stdint.h>
#include <string>

#include "util/lock.h"
#include "util/thread.h"

namespace util {

class LoopThread;

typedef std::shared_ptr<LoopThread> LoopThreadPtr;

class LoopThread {
private:
    explicit LoopThread(bool strictMode = false);

public:
    ~LoopThread();

private:
    LoopThread(const LoopThread &);
    LoopThread &operator=(const LoopThread &);

public:
    void Stop();

public:
    // if create loop thread fail, return null LoopThreadPtr
    static LoopThreadPtr
    CreateLoopThread(const std::function<void()> &loopFunction, int64_t loopInterval /*us*/, bool strictMode = false);
    // add this function to avoid the cast from (const char *) to (bool)
    static LoopThreadPtr CreateLoopThread(const std::function<void()> &loopFunction,
                                          int64_t loopInterval /*us*/,
                                          const char *name,
                                          bool strictMode = false);
    static LoopThreadPtr CreateLoopThread(const std::function<void()> &loopFunction,
                                          int64_t loopInterval /*us*/,
                                          const std::string &name,
                                          bool strictMode = false);
    void RunOnce();

public:
    static void DisableLoopFuncForTest() { disable_loop_fun_ = true; }
    static void EnableLoopFuncForTest() { disable_loop_fun_ = false; }

private:
    void Loop();

private:
    volatile bool run_;
    util::ThreadCond cond_;
    ThreadPtr thread_ptr_;
    std::string name_;
    std::function<void()> loop_function_;
    int64_t loop_interval_;
    int64_t next_time_;
    bool strict_mode_;
    static bool disable_loop_fun_;
};

} // namespace util
