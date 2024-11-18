#include "agent_engine/network/arpc/arpc/MessageCodec.h"

#include "agent_engine/network/arpc/arpc/MessageSerializable.h"
#include "agent_engine/network/arpc/arpc/PacketArg.h"
#include "agent_engine/network/arpc/arpc/RPCMessageSerializable.h"
#include "agent_engine/network/arpc/arpc/Tracer.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"

ARPC_BEGIN_NAMESPACE(arpc);
ARPC_DECLARE_AND_SETUP_LOGGER(MessageCodec);

MessageCodec::MessageCodec() {}

MessageCodec::~MessageCodec() {}

CallId MessageCodec::GenerateCallId(const RPCMethodDescriptor *method, version_t version) const {
    CallId callId;
    callId.intId = PacketCodeBuilder()(method);
    return callId;
}

ARPC_END_NAMESPACE(arpc);
