#include "util/TimeUtility.h"

#include <bits/types/struct_tm.h>
#include <iosfwd>

using namespace std;

namespace util {

int64_t TimeUtility::CurrentTime() {
    struct timeval tval;
    gettimeofday(&tval, NULL);
    return (tval.tv_sec * 1000000LL + tval.tv_usec);
}

int64_t TimeUtility::CurrentTimeInNanoSeconds() { return CurrentTime() * 1000; }

int64_t TimeUtility::CurrentTimeInSeconds() { return CurrentTime() / 1000000; }

int64_t TimeUtility::CurrentTimeInMicroSeconds() { return CurrentTime(); }

int64_t TimeUtility::CurrentTimeInMilliSeconds() { return CurrentTime() / 1000; }

int64_t TimeUtility::MonotonicTimeUs() { return MonotonicTimeNs() / 1000; }

int64_t TimeUtility::MonotonicTimeNs() {
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    return spec.tv_sec * 1000000000L + spec.tv_nsec;
}

int64_t TimeUtility::GetTime(int64_t usecOffset) { return CurrentTime() + usecOffset; }

timeval TimeUtility::GetTimeval(int64_t usecOffset) {
    timeval tval;
    int64_t uTime = GetTime(usecOffset);
    tval.tv_sec = uTime / 1000000;
    tval.tv_usec = uTime % 1000000;
    return tval;
}

timespec TimeUtility::GetTimespec(int64_t usecOffset) {
    timespec tspec;
    int64_t uTime = GetTime(usecOffset);
    tspec.tv_sec = uTime / 1000000;
    tspec.tv_nsec = (uTime % 1000000) * 1000;
    return tspec;
}

string TimeUtility::CurrentTimeString() {
    time_t lt = time(NULL);
    struct tm tim;
    localtime_r(&lt, &tim);
    char str[200];
    strftime(str, sizeof(str), "%Y%m%d%H%M%S", &tim);
    return string(str);
}

string TimeUtility::CurrentTimeString(const std::string &format) {
    time_t lt = time(NULL);
    struct tm tim;
    localtime_r(&lt, &tim);
    char str[200];
    strftime(str, sizeof(str), format.c_str(), &tim);
    return string(str);
}

} // namespace util
