#pragma once

#include <map>
#include <memory>
#include <string>

#include "agent_engine/network/arpc/arpc/metric/MetricReporterConfig.h"
#include "util/log.h"

namespace arpc {

class ClientRPCStats;

class ClientMetricReporter : public std::enable_shared_from_this<ClientMetricReporter> {
public:
    ClientMetricReporter(const MetricReporterConfig &config);
    virtual ~ClientMetricReporter() = default;

public:
    std::shared_ptr<ClientRPCStats> makeRPCStats();
    void reportRPCStats(ClientRPCStats *stats);

protected:
    virtual void doReportMetric(ClientRPCStats *stats) = 0;

protected:
    bool _enable{false};
    MetricReporterConfig _config;

private:
    LOG_DECLARE();
};

} // namespace arpc