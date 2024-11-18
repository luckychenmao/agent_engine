#include "agent_engine/network/arpc/arpc/metric/ServerRPCStats.h"

namespace arpc {

ServerRPCStats::~ServerRPCStats() {
    auto metricReporter = _metricReporter.lock();
    if (metricReporter) {
        metricReporter->reportRPCStats(this);
    }
}

void ServerRPCStats::markRequestDone(arpc::ErrorCode error) {
    _error = error;
    _doneTimeUs = util::TimeUtility::CurrentTimeInMicroSeconds();
}

} // namespace arpc