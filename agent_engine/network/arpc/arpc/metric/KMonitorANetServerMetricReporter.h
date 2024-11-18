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
    LOG_DECLARE();
};

} // namespace arpc