#include "util/loop_thread.h"

#include "util/log.h"
#include "util/time_utility.h"

namespace util {
DECLARE_AND_SETUP_LOGGER(util, LoopThread);

bool LoopThread::disable_loop_fun_ = false;
LoopThread::LoopThread(bool strictMode) {
    run_ = false;
    loop_interval_ = 0;
    next_time_ = 0;
    strict_mode_ = strictMode;
}

LoopThread::~LoopThread() { Stop(); }

LoopThreadPtr
LoopThread::CreateLoopThread(const std::function<void()> &loopFunction, int64_t loopInterval, bool strictMode) {
    return CreateLoopThread(loopFunction, loopInterval, std::string(), strictMode);
}

LoopThreadPtr LoopThread::CreateLoopThread(const std::function<void()> &loopFunction,
                                           int64_t loopInterval /*us*/,
                                           const char *name,
                                           bool strictMode) {
    return CreateLoopThread(loopFunction, loopInterval, std::string(name), strictMode);
}

LoopThreadPtr LoopThread::CreateLoopThread(const std::function<void()> &loopFunction,
                                           int64_t loopInterval /*us*/,
                                           const std::string &name,
                                           bool strictMode) {
    LoopThreadPtr ret(new LoopThread(strictMode));

    ret->name_ = name;
    ret->loop_function_ = loopFunction;
    ret->loop_interval_ = loopInterval;
    ret->run_ = true;

    ret->thread_ptr_ = Thread::CreateThread(std::bind(&LoopThread::Loop, ret.get()), name);

    if (!ret->thread_ptr_) {
        LOG(ERROR, "create loop thread fail");
        return LoopThreadPtr();
    }

    return ret;
}

void LoopThread::Stop() {
    {

        ScopedLock lock(cond_);
        if (!thread_ptr_) {
            return;
        }

        run_ = false;
        cond_.signal();
    }

    thread_ptr_->Join();
    thread_ptr_.reset();
}

void LoopThread::RunOnce() {
    ScopedLock lock(cond_);
    next_time_ = -1;
    cond_.signal();
}

void LoopThread::Loop() {
    LOG(INFO, "loop started for thread[%s] loopInterval[%ld]", name_.c_str(), loop_interval_);
    while (true) {
        int64_t nowTime;
        {
            ScopedLock lock(cond_);
            if (!run_) {
                break;
            }
            nowTime = TimeUtility::MonotonicTimeUs();
            if (nowTime < next_time_) {
                cond_.wait(next_time_ - nowTime);
                if (!run_) {
                    break;
                }
            }

            if (next_time_ == -1) {
                next_time_ = 0;
            }

            nowTime = TimeUtility::MonotonicTimeUs();
            if (nowTime < next_time_) {
                continue;
            }
        }
        if (!disable_loop_fun_) {
            loop_function_();
        }
        {
            // in strictMode, we reset the nowTime after loopFunction
            if (strict_mode_) {
                nowTime = TimeUtility::MonotonicTimeUs();
            }
            ScopedLock lock(cond_);
            if (next_time_ != -1) {
                next_time_ = nowTime + loop_interval_;
            }
        }
    }
}

} // namespace util
