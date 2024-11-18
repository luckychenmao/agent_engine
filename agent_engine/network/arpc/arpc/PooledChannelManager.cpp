
#include "arpc/PooledChannelManager.h"

#include "arpc/ANetRPCChannel.h"
#include "arpc/ANetRPCChannelManager.h"
#include "agent_engine/util/string_util.h"

using namespace std;
using namespace util;

namespace arpc {

LOG_SETUP(arpc, PooledChannelManager);

PooledChannelManager::PooledChannelManager(int ioThreadNum) {
    _transport = make_unique<anet::Transport>(ioThreadNum);
    _transport->start();
    _rpcChannelManager = make_unique<ANetRPCChannelManager>(_transport.get());
}

PooledChannelManager::~PooledChannelManager() {
    _loopThreadPtr.reset();
    _rpcChannelPool.clear();
    _transport->stop();
    _transport->wait();
}

shared_ptr<ANetRPCChannel> PooledChannelManager::getRpcChannel(const std::string &addr) {
    unique_lock<mutex> lock(_mu);
    if (_loopThreadPtr.get() == NULL) {
        _loopThreadPtr = LoopThread::createLoopThread(
            bind(&PooledChannelManager::cleanBrokenChannelLoop, this), 10 * 1000 * 1000, "cleanChannel");
    }
    auto it = _rpcChannelPool.find(addr);
    if (it != _rpcChannelPool.end()) {
        auto ch = it->second.get();
        if (ch->ChannelBroken()) {
            _rpcChannelPool.erase(it);
            return nullptr;
        } else {
            return it->second;
        }
    }

    shared_ptr<ANetRPCChannel> rpcChannel = createRpcChannel(addr);
    if (rpcChannel) {
        _rpcChannelPool[addr] = rpcChannel;
    }

    return rpcChannel;
}

void PooledChannelManager::cleanBrokenChannelLoop() {
    unique_lock<mutex> lock(_mu);
    for (auto it = _rpcChannelPool.begin(); it != _rpcChannelPool.end();) {
        auto ch = it->second.get();
        if (ch->ChannelBroken()) {
            _rpcChannelPool.erase(it++);
        } else {
            it++;
        }
    }
}

shared_ptr<ANetRPCChannel> PooledChannelManager::createRpcChannel(const string &addr) {
    string spec = convertToSpec(addr);
    bool block = false;
    size_t queueSize = 50ul;
    bool autoReconnect = false;
    shared_ptr<ANetRPCChannel> rpcChannel(
        (ANetRPCChannel *)_rpcChannelManager->OpenChannel(spec, block, queueSize, _timeout, autoReconnect));
    if (rpcChannel == NULL) {
        LOG(ERROR, "open channel on [%s] failed", spec.c_str());
    }
    return rpcChannel;
}

string PooledChannelManager::convertToSpec(const string &addr) {
    if (StringUtil::StartsWith(addr, "tcp:") || StringUtil::StartsWith(addr, "udp:") ||
        StringUtil::StartsWith(addr, "http:")) {
        return addr;
    } else {
        return "tcp:" + addr;
    }
}

} // namespace arpc
