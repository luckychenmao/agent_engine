#ifndef ARPC_UTIL_FUN_H
#define ARPC_UTIL_FUN_H
#include <memory>
#include <string>

#include "agent_engine/network/anet/anet.h"
#include "agent_engine/network/arpc/arpc/ANetRPCController.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/MessageSerializable.h"
#include "agent_engine/network/arpc/arpc/proto/rpc_extensions.pb.h"

ARPC_BEGIN_NAMESPACE(arpc);

ErrorMsg *
BuildErrorMsg(const std::string &errMsg, ErrorCode errCode, const std::shared_ptr<google::protobuf::Arena> &arena);

anet::DelayDecodePacket *BuildPacket(anet::PacketHeader *header, MessageSerializable *pSeri, bool ownSeri = true);

void DecodePacket(ANetRPCController *pController,
                  anet::DelayDecodePacket *pPacket,
                  RPCMessage *message,
                  const std::shared_ptr<google::protobuf::Arena> &arenaPtr,
                  bool bResponse = true);

ARPC_END_NAMESPACE(arpc);

#endif // ARPC_UTIL_FUN_H
