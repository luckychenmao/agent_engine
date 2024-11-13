#pragma once

#include "util/log.h"

namespace network {

class ChannelPool {
public:
    ChannelPool();
    ~ChannelPool();
    ChannelPool(const ChannelPool &) = delete;
    ChannelPool &operator=(const ChannelPool &) = delete;

public:

private:
    LOG_DECLARE();

};

} // namespace network
