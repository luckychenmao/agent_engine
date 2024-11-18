#ifndef ARPC_ANETRPCMESSAGECODEC_H
#define ARPC_ANETRPCMESSAGECODEC_H
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/MessageCodec.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"

ARPC_BEGIN_NAMESPACE(arpc);

class ANetRPCMessageCodec : public MessageCodec {
public:
    ANetRPCMessageCodec(anet::IPacketFactory *packetFactory);
    ~ANetRPCMessageCodec();

public:
    anet::Packet *EncodeRequest(const CodecContext *context, version_t version) const;
    CodecContext *DecodeRequest(anet::Packet *packet, const RPCServer *rpcServer, version_t version) const;
    anet::Packet *EncodeResponse(RPCMessage *response,
                                 Tracer *tracer,
                                 version_t version,
                                 const std::shared_ptr<google::protobuf::Arena> &arena) const;

protected:
    anet::Packet *createPacket(uint32_t pcode, anet::DataBufferSerializable *serializable) const;

private:
    friend class ANetRPCMessageCodecTest;

protected:
    anet::IPacketFactory *_packetFactory;
};

TYPEDEF_PTR(ANetRPCMessageCodec);
ARPC_END_NAMESPACE(arpc);

#endif // ARPC_ANETRPCMESSAGECODEC_H
