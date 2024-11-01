#pragma once

#include <algorithm>
#include <assert.h>
#include <limits>
#include <stdint.h>
#include <string>
#include <sys/time.h>
#include <time.h>

namespace util {
inline int64_t TimeAdd(int64_t startTime, int64_t diff) {
    assert(startTime >= 0);
    // avoid op plus overflow
    int64_t timeoutThreshold = std::numeric_limits<int64_t>::max() - startTime;
    diff = std::min(diff, timeoutThreshold);
    return startTime + diff;
}

class TimeUtility {
public:
    static int64_t CurrentTime(); // us
    static int64_t CurrentTimeInSeconds();
    static int64_t CurrentTimeInMilliSeconds(); // ms
    static int64_t CurrentTimeInMicroSeconds(); // us
    static int64_t CurrentTimeInNanoSeconds();  // ns
    static int64_t MonotonicTimeUs();
    static int64_t MonotonicTimeNs();

    static int64_t GetTime(int64_t usecOffset = 0);
    static timeval GetTimeval(int64_t usecOffset = 0);
    static timespec GetTimespec(int64_t usecOffset = 0);
    static int64_t UsToMs(int64_t us) { return us / 1000; }
    static int64_t MsToUs(int64_t ms) { return ms * 1000; }
    static int64_t MsToSec(int64_t ms) { return ms / 1000; }
    static int64_t SecToMs(int64_t sec) { return sec * 1000; }
    static int64_t UsToSec(int64_t us) { return us / 1000000; }
    static int64_t SecToUs(int64_t sec) { return sec * 1000000; }
    static std::string CurrentTimeString();
    static std::string CurrentTimeString(const std::string &format);
};

class StageTime {
public:
    StageTime() { begin_time_ = TimeUtility::MonotonicTimeUs(); }
    ~StageTime() = default;
    void end_stage() {
        int64_t endTime = TimeUtility::MonotonicTimeUs();
        last_stage_us_ = endTime - begin_time_;
        begin_time_ = endTime;
    }
    double last_sec() const { return TimeUtility::UsToSec(last_us()); }
    double last_ms() const { return TimeUtility::UsToMs(last_us()); }
    int64_t last_us() const { return last_stage_us_; }

private:
    int64_t begin_time_;
    int64_t last_stage_us_;
};

} // namespace util
