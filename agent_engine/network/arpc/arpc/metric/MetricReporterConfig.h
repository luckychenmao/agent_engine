#pragma once

#include "agent_engine/util/legacy/jsonizable.h"

namespace arpc {

class MetricReporterConfig : public util::legacy::Jsonizable
{
public:
    MetricReporterConfig(bool enable = false);
    ~MetricReporterConfig() = default;

public:
    void Jsonize(util::legacy::Jsonizable::JsonWrapper& json) override;

public:
    bool enableArpcMetric {false};
    bool enableSlowRequestLog {false};
    uint64_t slowRequestThresholdInUs {20 * 1000};
    std::string type{"unknown"};
};

} // namespace arpc