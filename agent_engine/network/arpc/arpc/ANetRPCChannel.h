#ifndef ARPC_ANET_RPC_CHANNEL_H
#define ARPC_ANET_RPC_CHANNEL_H

#include "agent_engine/network/anet/anet.h"
#include "agent_engine/network/arpc/arpc/ANetRPCController.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/RPCChannelBase.h"
#include "agent_engine/network/arpc/arpc/anet/ANetRPCMessageCodec.h"
#include "agent_engine/network/arpc/arpc/anet/ClientPacketHandler.h"
#include "agent_engine/network/arpc/arpc/anet/SharedClientPacketHandler.h"
#include "agent_engine/network/arpc/arpc/metric/ClientMetricReporter.h"

ARPC_BEGIN_NAMESPACE(arpc);

class ANetRPCChannel : public RPCChannelBase {
public:
    ANetRPCChannel(anet::Connection *pConnection, ANetRPCMessageCodec *messageCode, bool block = false);
    virtual ~ANetRPCChannel();

public:
    virtual void CallMethod(const RPCMethodDescriptor *method,
                            RPCController *controller,
                            const RPCMessage *request,
                            RPCMessage *response,
                            RPCClosure *done) override;
    virtual bool ChannelBroken() override;
    virtual bool ChannelConnected() override;
    bool CheckResponsePacket(anet::Packet *packet, RpcReqArg *pArgs);
    std::string getRemoteAddr() const;
    void SetMetricReporter(const std::shared_ptr<ClientMetricReporter> &metricReporter);

public: // for test
    void SyncCall(anet::Packet *pPack, RpcReqArg *pArg);
    bool AsyncCall(anet::Packet *pPack, RpcReqArg *pArg);
    SharedClientPacketHandler *getSharedHandlerFromConnection();

protected:
    virtual bool PostPacket(anet::Packet *packet, anet::IPacketHandler *packetHandler, void *args, bool block);

    bool needRepostPacket(ErrorCode errorCode, version_t remoteVersion, version_t postedPacketVersion) const;

protected:
    ClientPacketHandler _handler;
    anet::Connection *_pConnection;
    bool _block;
    ANetRPCMessageCodec *_messageCodec;
    std::shared_ptr<ClientMetricReporter> _metricReporter;

private:
    friend class ANetRPCChannelTest;
};

ARPC_END_NAMESPACE(arpc);
#endif // ARPC_ANET_RPC_CHANNEL_H
