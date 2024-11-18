#include "agent_engine/network/arpc/arpc/RPCChannelBase.h"

#include <cstddef>
#include <stdint.h>
#include <unistd.h>

#include "agent_engine/network/anet/advancepacket.h"
#include "agent_engine/network/anet/connection.h"
#include "agent_engine/network/anet/connectionpriority.h"
#include "agent_engine/network/anet/ilogger.h"
#include "agent_engine/network/anet/ipackethandler.h"
#include "agent_engine/network/anet/packet.h"
#include "agent_engine/network/anet/runnable.h"
#include "agent_engine/network/arpc/arpc/ANetRPCController.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/MessageCodec.h"
#include "agent_engine/network/arpc/arpc/PacketArg.h"
#include "agent_engine/network/arpc/arpc/Tracer.h"
#include "agent_engine/network/arpc/arpc/anet/SharedClientPacketHandler.h"
#include "agent_engine/network/arpc/arpc/proto/rpc_extensions.pb.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"

using namespace std;
using namespace anet;
ARPC_BEGIN_NAMESPACE(arpc);
ARPC_DECLARE_AND_SETUP_LOGGER(RPCChannelBase);

RPCChannelBase::RPCChannelBase() {
    _version = ARPC_VERSION_CURRENT;
    _enableTrace = false;
}

RPCChannelBase::~RPCChannelBase() {}

void RPCChannelBase::SetError(ANetRPCController *pController, ErrorCode errorCode) {
    pController->SetErrorCode(errorCode);
    pController->SetFailed(errorCodeToString(errorCode));
    pController->GetTracer().EndCallMethod(errorCode);
    return;
}

void RPCChannelBase::RunClosure(RPCClosure *pClosure) {
    if (pClosure != NULL) {
        pClosure->Run();
    }
}

void RPCChannelBase::SetTraceFlag(ANetRPCController *controller) {
    if (GetTraceFlag() && controller) {
        controller->SetTraceFlag(true);
    }
}

ARPC_END_NAMESPACE(arpc);
