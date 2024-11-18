#pragma once

#include "agent_engine/network/anet/metric/KMonitorANetMetricReporter.h"
#include "agent_engine/network/arpc/arpc/metric/KMonitorANetMetricReporterConfig.h"
#include "agent_engine/network/arpc/arpc/metric/KMonitorClientMetricReporter.h"
#include "util/log.h"

namespace arpc {

class KMonitorANetClientMetricReporter : public ClientMetricReporter {
public:
    KMonitorANetClientMetricReporter(const KMonitorANetMetricReporterConfig &config);
    ~KMonitorANetClientMetricReporter() override = default;

public:
    void doReportMetric(ClientRPCStats *stats) override;
    bool init(anet::Transport *transport);

private:
    KMonitorANetMetricReporterConfig _config;
    std::shared_ptr<KMonitorClientMetricReporter> _arpcReporter;
    std::shared_ptr<anet::KMonitorANetMetricReporter> _anetReporter;

private:
    LOG_DECLARE();
};

} // namespace arpc