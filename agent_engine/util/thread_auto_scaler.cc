/*
 * Copyright 2014-present Alibaba Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "util/thread_auto_scaler.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <errno.h>
#include <functional>
#include <memory>
#include <unistd.h>
#include <vector>

#include "util/env_util.h"
#include "util/lock.h"
#include "util/log.h"
#include "util/string_util.h"

namespace util {

DECLARE_AND_SETUP_LOGGER(util, ThreadAutoScaler);

const std::string ThreadAutoScaler::THREAD_NUM_RANGE_SEPERATOR = ":";
const std::string ThreadAutoScaler::THREAD_NUM_RANGE_ENV = "arpc_auto_scale_thread_num_range";

ThreadAutoScaler::ThreadAutoScaler()
    : mMinActiveThreadNum(0)
    , mMaxActiveThreadNum(0)
    , mActiveThreadCount(0)
    , mCpuScaleNotifier(nullptr)
    , mStopped(false) {}

ThreadAutoScaler::~ThreadAutoScaler() {
    Stop();
    delete[] mCpuScaleNotifier;
}

bool ThreadAutoScaler::Init(const std::string &threadName) {
    if (!InitByEnv()) {
        return false;
    }
    if (!UpdateActiveThreadCount()) {
        return false;
    }
    mCpuScaleNotifier = new util::ThreadCond[mMaxActiveThreadNum];
    mAutoScaleThread =
        util::Thread::CreateThread(std::bind(&ThreadAutoScaler::AutoScaleThread, this), threadName + "AutoScale");
    if (!mAutoScaleThread) {
        LOG(ERROR, "create auto scale thread failed");
        return false;
    }
    LOG(INFO,
              "auto scale init success, active [%lu], thread num range min[%lu], max[%lu]",
              mActiveThreadCount,
              mMinActiveThreadNum,
              mMaxActiveThreadNum);
    return true;
}

bool ThreadAutoScaler::InitByEnv() {
    std::string rangeStr = util::EnvUtil::GetEnv(THREAD_NUM_RANGE_ENV);
    if (rangeStr.empty()) {
        mMinActiveThreadNum = DEFAULT_THREAD_NUM_MIN;
        mMaxActiveThreadNum = DEFAULT_THREAD_NUM_MAX;
        return true;
    }
    auto splited = StringUtil::Split(std::string(rangeStr), THREAD_NUM_RANGE_SEPERATOR);
    if (2u != splited.size()) {
        LOG(ERROR, "invalid thread num range[%s], field count is not 2", rangeStr.c_str());
        return false;
    }
    const auto &minStr = splited[0];
    const auto &maxStr = splited[1];
    size_t minFromEnv = 0;
    size_t maxFromEnv = 0;
    if (!StringUtil::StrToUInt64(minStr.c_str(), minFromEnv) || !StringUtil::StrToUInt64(maxStr.c_str(), maxFromEnv)) {
        LOG(ERROR, "invalid range[%s], parse number failed", rangeStr.c_str());
        return false;
    }
    if (maxFromEnv < minFromEnv) {
        LOG(
            ERROR, "invalid range[%s], max[%lu] is smaller than min[%lu]", rangeStr.c_str(), maxFromEnv, minFromEnv);
        return false;
    }
    mMinActiveThreadNum = minFromEnv;
    mMaxActiveThreadNum = maxFromEnv;
    LOG(INFO, "get thread num range from env [%s] success", rangeStr.c_str());
    return true;
}

void ThreadAutoScaler::Stop() {
    mActiveThreadCount = mMaxActiveThreadNum;
    Signal();
    mStopped = true;
    if (mAutoScaleThread) {
        mAutoScaleThread->Join();
        mAutoScaleThread.reset();
    }
}

void ThreadAutoScaler::Wait(size_t index) {
    if (index < mActiveThreadCount) {
        // active
        return;
    }
    if (index < mMaxActiveThreadNum && mCpuScaleNotifier) {
        // inactive
        LOG(INFO, "thread [%lu] suspended", index);
        auto &cond = mCpuScaleNotifier[index];
        cond.lock();
        cond.wait();
        cond.unlock();
        LOG(INFO, "thread [%lu] resumed", index);
    } else {
        // never reach
        assert(false);
    }
}

void ThreadAutoScaler::AutoScaleThread() {
    while (true) {
        if (mStopped) {
            break;
        }
        UpdateActiveThreadCount();
        Signal();
        usleep(1000 * 1000);
    }
}

void ThreadAutoScaler::Signal() {
    if (!mCpuScaleNotifier) {
        return;
    }
    size_t activeCount = mActiveThreadCount;
    activeCount = std::min(activeCount, mMaxActiveThreadNum);
    for (size_t i = 0; i < activeCount; i++) {
        auto &cond = mCpuScaleNotifier[i];
        cond.lock();
        cond.signal();
        cond.unlock();
    }
}

bool ThreadAutoScaler::UpdateActiveThreadCount() {
    size_t newThreadCount = mMaxActiveThreadNum;
    if (!CalcActiveThreadCount(newThreadCount)) {
        return false;
    }
    newThreadCount = std::min(newThreadCount, mMaxActiveThreadNum);
    newThreadCount = std::max(newThreadCount, mMinActiveThreadNum);
    if (newThreadCount != mActiveThreadCount) {
        LOG(INFO, "active thread count changed from[%lu] to [%lu]", mActiveThreadCount, newThreadCount);
    }
    mActiveThreadCount = newThreadCount;
    return true;
}

bool ThreadAutoScaler::CalcActiveThreadCount(size_t &activeCount) {
    errno = 0;
    auto coreNum = sysconf(_SC_NPROCESSORS_ONLN);
    if (errno == 0 && coreNum > 0) {
        activeCount = coreNum;
        return true;
    } else {
        LOG(ERROR, "auto scale get core num failed");
        return false;
    }
}

size_t ThreadAutoScaler::GetMaxThreadNum() const { return mMaxActiveThreadNum; }

size_t ThreadAutoScaler::GetMinThreadNum() const { return mMinActiveThreadNum; }

size_t ThreadAutoScaler::GetActiveThreadNum() const { return mActiveThreadCount; }

} // namespace util
