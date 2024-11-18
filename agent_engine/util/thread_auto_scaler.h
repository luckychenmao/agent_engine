#pragma once

#include <cstddef>
#include <string>

#include "util/lock.h"
#include "util/thread.h"

namespace util {

class ThreadAutoScaler {
public:
    ThreadAutoScaler();
    ~ThreadAutoScaler();

public:
    bool Init(const std::string &threadName);
    void Stop();
    void Wait(size_t index);
    size_t GetMaxThreadNum() const;
    size_t GetMinThreadNum() const;
    size_t GetActiveThreadNum() const;

private:
    bool InitByEnv();
    void AutoScaleThread();
    bool UpdateActiveThreadCount();
    void Signal();

private:
    static bool CalcActiveThreadCount(size_t &activeCount);

private:
    size_t mMinActiveThreadNum;
    size_t mMaxActiveThreadNum;
    volatile size_t mActiveThreadCount;
    util::ThreadCond *mCpuScaleNotifier;
    bool mStopped;
    util::ThreadPtr mAutoScaleThread;

private:
    static constexpr size_t DEFAULT_THREAD_NUM_MIN = 20;
    static constexpr size_t DEFAULT_THREAD_NUM_MAX = 96;
    static const std::string THREAD_NUM_RANGE_SEPERATOR;

public:
    static const std::string THREAD_NUM_RANGE_ENV;
};

} // namespace util
