#ifndef ARPC_CLIENTPACKETHANDLER_H
#define ARPC_CLIENTPACKETHANDLER_H
#include <memory>
#include <stdint.h>

#include "agent_engine/network/anet/anet.h"
#include "agent_engine/network/anet/ipackethandler.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/PacketArg.h"
#include "agent_engine/network/arpc/arpc/proto/rpc_extensions.pb.h"

ARPC_BEGIN_NAMESPACE(arpc);
class RPCChannelBase;
class ANetRPCChannel;

class ClientPacketHandler : public anet::IPacketHandler {
public:
    ClientPacketHandler();
    virtual ~ClientPacketHandler();

public:
    anet::IPacketHandler::HPRetCode handlePacket(anet::Packet *packet, void *args);
    void setChannel(ANetRPCChannel *channel) { _channel = channel; }

public:
    // for test
    RPCChannelBase *getChannel();

protected:
    anet::IPacketHandler::HPRetCode handleCmdPacket(anet::Packet *packet, RpcReqArg *pArgs);
    void decodePacket(ANetRPCController *controller,
                      anet::Packet *packet,
                      RPCMessage *response,
                      const std::shared_ptr<google::protobuf::Arena> &arenaPtr);
    bool needRepostPacket(anet::Packet *packet);
    void repostPacket(anet::Packet *packet, void *args);
    anet::DataBufferSerializable *createSerializable(int32_t pcode,
                                                     RPCMessage *response,
                                                     TraceInfo *traceInfo,
                                                     version_t version,
                                                     google::protobuf::Arena *arena,
                                                     const std::shared_ptr<google::protobuf::Arena> &arenaPtr);

protected:
    ANetRPCChannel *_channel;

private:
    friend class ClientPacketHandlerTest;
};

ARPC_END_NAMESPACE(arpc);

#endif // ARPC_CLIENTPACKETHANDLER_H
