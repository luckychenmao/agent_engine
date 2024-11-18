#pragma once
#include <memory>

#include "agent_engine/network/anet/anet.h"
#include "agent_engine/network/arpc/arpc/ANetRPCController.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/RPCServerClosure.h"
#include "agent_engine/network/arpc/arpc/anet/ANetRPCMessageCodec.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"
#include "google/protobuf/arena.h"

ARPC_BEGIN_NAMESPACE(arpc);

class ANetRPCServerClosure : public RPCServerClosure {
public:
    ANetRPCServerClosure(channelid_t chid,
                         anet::Connection *connection,
                         RPCMessage *reqMsg,
                         RPCMessage *resMsg,
                         ANetRPCMessageCodec *messageCodec,
                         version_t version);

    virtual ~ANetRPCServerClosure();

    std::string getIpAndPortAddr() override;

public:
    // for test
    anet::Connection *GetConnection() const { return _connection; }

    channelid_t GetChid() const { return _chid; }
    anet::Packet *buildResponsePacket();

protected:
    virtual void doPostPacket() override;
    friend class RPCServerClosureTest;

private:
    // need by anet
    channelid_t _chid;
    anet::Connection *_connection;
    ANetRPCMessageCodec *_messageCodec;
    version_t _version;
};

TYPEDEF_PTR(RPCServerClosure);

ARPC_END_NAMESPACE(arpc);