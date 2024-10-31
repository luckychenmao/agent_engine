#pragma once

#include <algorithm>
#include <assert.h>
#include <limits>
#include <stdint.h>
#include <string>
#include <sys/time.h>
#include <time.h>

namespace util {

template <typename T>
float US_TO_MS(T us) {
    return us / 1000.0;
}
template <typename T>
float US_TO_SEC(T us) {
    return us / 1000.0 / 1000.0;
}

inline int64_t TIME_ADD(int64_t startTime, int64_t diff) {
    assert(startTime >= 0);
    // avoid op plus overflow
    int64_t timeoutThreshold = std::numeric_limits<int64_t>::max() - startTime;
    diff = std::min(diff, timeoutThreshold);
    return startTime + diff;
}

class TimeUtility {
public:
    static int64_t currentTime(); // us
    static int64_t currentTimeInSeconds();
    static int64_t currentTimeInMilliSeconds(); // ms
    static int64_t currentTimeInMicroSeconds(); // us
    static int64_t currentTimeInNanoSeconds();  // ns
    static int64_t monotonicTimeUs();
    static int64_t monotonicTimeNs();

    static int64_t getTime(int64_t usecOffset = 0);
    static timeval getTimeval(int64_t usecOffset = 0);
    static timespec getTimespec(int64_t usecOffset = 0);
    static int64_t us2ms(int64_t us) { return us / 1000; }
    static int64_t ms2us(int64_t ms) { return ms * 1000; }
    static int64_t ms2sec(int64_t ms) { return ms / 1000; }
    static int64_t sec2ms(int64_t sec) { return sec * 1000; }
    static int64_t us2sec(int64_t us) { return us / 1000000; }
    static int64_t sec2us(int64_t sec) { return sec * 1000000; }
    static std::string currentTimeString();
    static std::string currentTimeString(const std::string &format);
    static std::string usFormat(int64_t us, const std::string &format);
};

class ScopedTime {
public:
    ScopedTime(int64_t &t) : _t(t) { _b = TimeUtility::monotonicTimeUs(); }
    ~ScopedTime() { _t += TimeUtility::monotonicTimeUs() - _b; }

private:
    int64_t &_t;
    int64_t _b;
};

class ScopedTime2 {
public:
    ScopedTime2() { _beginTime = TimeUtility::monotonicTimeUs(); }
    ~ScopedTime2() {}
    double done_sec() const {
        int64_t endTime = TimeUtility::monotonicTimeUs();
        return US_TO_SEC(endTime - _beginTime);
    }
    double done_ms() const {
        int64_t endTime = TimeUtility::monotonicTimeUs();
        return US_TO_MS(endTime - _beginTime);
    }
    int64_t done_us() const {
        int64_t endTime = TimeUtility::monotonicTimeUs();
        return endTime - _beginTime;
    }
    int64_t begin_us() const { return TimeUtility::currentTime() - done_us(); }

private:
    int64_t _beginTime;
};

class StageTime {
public:
    StageTime() { _beginTime = TimeUtility::monotonicTimeUs(); }
    ~StageTime() = default;
    void end_stage() {
        int64_t endTime = TimeUtility::monotonicTimeUs();
        _lastStageUs = endTime - _beginTime;
        _beginTime = endTime;
    }
    double last_sec() const { return US_TO_SEC(last_us()); }
    double last_ms() const { return US_TO_MS(last_us()); }
    int64_t last_us() const { return _lastStageUs; }

private:
    int64_t _beginTime;
    int64_t _lastStageUs;
};

} // namespace util
