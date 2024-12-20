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
#include "agent_engine/network/arpc/arpc/metric/ClientRPCStats.h"

namespace arpc {

AUTIL_LOG_SETUP(arpc, ClientRPCStats);

ClientRPCStats::~ClientRPCStats() {
    auto metricReporter = _metricReporter.lock();
    if (metricReporter != nullptr) {
        metricReporter->reportRPCStats(this);
    }
}

void ClientRPCStats::markRequestDone(arpc::ErrorCode error) {
    _error = error;
    _doneTimeUs = util::TimeUtility::CurrentTimeInMicroSeconds();
}

} // namespace arpc