#include "agent_engine/network/arpc/arpc/metric/MetricReporterConfig.h"

#include "agent_engine/util/env_util.h"

namespace arpc {

const std::string kReportArpcMetricsEnvName = "ARPC_METRIC_ENABLE";
const std::string kShowSlowRequestLogEnvName = "ARPC_SLOW_REQUEST_LOG";
const std::string kSlowRequestThresholdInUsEnvName = "ARPC_SLOW_REQUEST_THRESHOLD_IN_US";

MetricReporterConfig::MetricReporterConfig(bool enable) : enableArpcMetric(enable) 
{
    enableArpcMetric = util::EnvUtil::GetEnv(kReportArpcMetricsEnvName, enableArpcMetric);
    enableSlowRequestLog = util::EnvUtil::GetEnv(kShowSlowRequestLogEnvName, enableSlowRequestLog);
    slowRequestThresholdInUs = util::EnvUtil::GetEnv(kSlowRequestThresholdInUsEnvName, slowRequestThresholdInUs);
}

void MetricReporterConfig::Jsonize(util::legacy::Jsonizable::JsonWrapper& json)
{
    json.Jsonize("enable_arpc_metric", enableArpcMetric, enableArpcMetric);
    json.Jsonize("enable_slow_request_log", enableSlowRequestLog, enableSlowRequestLog);
    json.Jsonize("slow_request_threshold_us", slowRequestThresholdInUs, slowRequestThresholdInUs);
    json.Jsonize("type", type, type);
}

} // namespace arpc