#include "agent_engine/network/arpc/arpc/metric/ClientMetricReporter.h"

#include "agent_engine/network/arpc/arpc/metric/ClientRPCStats.h"

namespace arpc {

LOG_SETUP(arpc, ClientMetricReporter);

ClientMetricReporter::ClientMetricReporter(const MetricReporterConfig &config) : _config(config) {
    if (_config.type.empty()) {
        _config.type = "unknown";
    }
    _enable = _config.enableArpcMetric || _config.enableSlowRequestLog;
}

std::shared_ptr<ClientRPCStats> ClientMetricReporter::makeRPCStats() {
    if (!_enable) {
        return nullptr;
    }
    return std::make_shared<ClientRPCStats>(shared_from_this());
}

void ClientMetricReporter::reportRPCStats(ClientRPCStats *stats) {
    if (stats == nullptr || !_enable) {
        return;
    }

    doReportMetric(stats);

    if (_config.enableSlowRequestLog && _config.slowRequestThresholdInUs < stats->totalCostUs()) {
        LOG(WARN,
                  "slow client request. requestId: %d, totalCostUs: %ld, requestPack: %ld, "
                  "requestSend: %ld, responsePop: %ld, responseUnpack: %ld",
                  stats->requestId(),
                  stats->totalCostUs(),
                  stats->requestPackCostUs(),
                  stats->requestSendCostUs(),
                  stats->responsePopCostUs(),
                  stats->responseUnpackCostUs());
    }
}

} // namespace arpc