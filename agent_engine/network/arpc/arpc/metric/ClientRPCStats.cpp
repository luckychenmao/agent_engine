#include "agent_engine/network/arpc/arpc/metric/ClientRPCStats.h"

namespace arpc {

LOG_SETUP(arpc, ClientRPCStats);

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