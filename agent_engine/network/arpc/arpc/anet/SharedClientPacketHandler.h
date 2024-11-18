#ifndef ARPC_SHAREDCLIENTPACKETHANDLER_H
#define ARPC_SHAREDCLIENTPACKETHANDLER_H
#include <functional>
#include <stdint.h>

#include "agent_engine/network/anet/ipackethandler.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/anet/ClientPacketHandler.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"

ARPC_BEGIN_NAMESPACE(arpc);

class SharedClientPacketHandler : public ClientPacketHandler {
public:
    SharedClientPacketHandler();

protected:
    virtual ~SharedClientPacketHandler();

public:
    anet::IPacketHandler::HPRetCode handlePacket(anet::Packet *packet, void *args);
    void addRef();
    void subRef();
    void cleanChannel();

private:
    anet::IPacketHandler::HPRetCode doHandlePacket(anet::Packet *packet, void *args);

private:
    // for test
    void setHandlePacketPreFunc(const std::function<void()> *func);
    friend class ANetRPCChannelCloseBugTest;

private:
    int64_t _ref;
    mutable std::mutex _mutex;
    mutable std::mutex _channelLock;
    const std::function<void()> *_func;
};

TYPEDEF_PTR(SharedClientPacketHandler);
ARPC_END_NAMESPACE(arpc);

#endif // ARPC_SHAREDCLIENTPACKETHANDLER_H
