#pragma once

#include <map>
#include <memory>
#include <string>

#include "agent_engine/network/arpc/arpc/metric/MetricReporterConfig.h"
#include "util/log.h"

namespace arpc {

class ServerRPCStats;

class ServerMetricReporter : public std::enable_shared_from_this<ServerMetricReporter> {
public:
    ServerMetricReporter(const MetricReporterConfig &config);
    virtual ~ServerMetricReporter() = default;

public:
    std::shared_ptr<ServerRPCStats> makeRPCStats();
    void reportRPCStats(ServerRPCStats *stats);

protected:
    virtual void doReportMetric(ServerRPCStats *stats) = 0;

protected:
    bool _enable{false};
    MetricReporterConfig _config;

private:
    LOG_DECLARE();
};

} // namespace arpc