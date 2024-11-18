#include "agent_engine/network/arpc/arpc/metric/KMonitorANetClientMetricReporter.h"

namespace arpc {

LOG_SETUP(arpc, KMonitorANetClientMetricReporter);

KMonitorANetClientMetricReporter::KMonitorANetClientMetricReporter(const KMonitorANetMetricReporterConfig &config)
    : ClientMetricReporter(config.arpcConfig), _config{config} {}

bool KMonitorANetClientMetricReporter::init(anet::Transport *transport) {
    if (!_enable) {
        return true;
    }

    _arpcReporter = std::make_shared<KMonitorClientMetricReporter>(_config.arpcConfig, _config.metricLevel);
    if (_arpcReporter == nullptr || !_arpcReporter->init()) {
        _enable = false;
        LOG(WARN, "kmonitor ANet client metric reporter init failed, arpc reporter init failed");
        return false;
    }

    _anetReporter = std::make_shared<anet::KMonitorANetMetricReporter>(_config.anetConfig, _config.metricLevel);
    if (_anetReporter == nullptr || !_anetReporter->init(transport)) {
        _enable = false;
        LOG(WARN, "kmonitor ANet client metric reporter init failed, accl reporter init failed");
        return false;
    }

    LOG(INFO, "kmonitor ANet client metric reporter init done");
    return true;
}

void KMonitorANetClientMetricReporter::doReportMetric(ClientRPCStats *stats) {
    if (_arpcReporter != nullptr && stats != nullptr) {
        _arpcReporter->doReportMetric(stats);
    }
}

} // namespace arpc