#pragma once

#include "util/log.h"

namespace network {

class Connection {
public:
    Connection();
    ~Connection();
    Connection(const Connection &) = delete;
    Connection &operator=(const Connection &) = delete;

public:
    virtual void post()

private:
    LOG_DECLARE();

};

} // namespace network
