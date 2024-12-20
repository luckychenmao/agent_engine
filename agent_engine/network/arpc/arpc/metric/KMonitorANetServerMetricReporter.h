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
#pragma once

#include "agent_engine/network/anet/metric/KMonitorANetMetricReporter.h"
#include "agent_engine/network/arpc/arpc/metric/KMonitorANetMetricReporterConfig.h"
#include "agent_engine/network/arpc/arpc/metric/KMonitorServerMetricReporter.h"
#include "util/log.h"

namespace arpc {

class KMonitorANetServerMetricReporter : public ServerMetricReporter {
public:
    KMonitorANetServerMetricReporter(const KMonitorANetMetricReporterConfig &config);
    ~KMonitorANetServerMetricReporter() override = default;

public:
    void doReportMetric(ServerRPCStats *stats) override;
    bool init(anet::Transport *transport);

private:
    KMonitorANetMetricReporterConfig _config;
    std::shared_ptr<KMonitorServerMetricReporter> _arpcReporter;
    std::shared_ptr<anet::KMonitorANetMetricReporter> _anetReporter;

private:
    AUTIL_LOG_DECLARE();
};

} // namespace arpc