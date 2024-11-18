#pragma once

#include <memory>
#include <mutex>

#include "util/log.h"
#include "agent_engine/util/loop_thread.h"

namespace anet {
class Transport;
}

namespace arpc {

class ANetRPCChannelManager;
class ANetRPCChannel;

class PooledChannelManager {
public:
    PooledChannelManager(int ioThreadNum = 1);
    virtual ~PooledChannelManager();
    PooledChannelManager(const PooledChannelManager &) = delete;
    PooledChannelManager &operator=(const PooledChannelManager &) = delete;

public:
    virtual std::shared_ptr<ANetRPCChannel> getRpcChannel(const std::string &addr);
    void setTimeout(int timeout) { _timeout = timeout; }

private:
    std::shared_ptr<ANetRPCChannel> createRpcChannel(const std::string &spec);
    void cleanBrokenChannelLoop();

private:
    static std::string convertToSpec(const std::string &addr);

private:
    std::mutex _mu;
    int _timeout = 5000;
    std::unique_ptr<anet::Transport> _transport;
    std::unique_ptr<arpc::ANetRPCChannelManager> _rpcChannelManager;
    std::map<std::string, std::shared_ptr<ANetRPCChannel>> _rpcChannelPool;
    util::LoopThreadPtr _loopThreadPtr;

private:
    LOG_DECLARE();
};

} // namespace arpc
