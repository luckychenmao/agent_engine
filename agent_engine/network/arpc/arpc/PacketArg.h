#ifndef ARPC_PACKETARG_H
#define ARPC_PACKETARG_H
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/stubs/port.h>
#include <stdint.h>

#include "agent_engine/network/arpc/arpc/ANetRPCController.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/MessageCodec.h"
#include "agent_engine/network/arpc/arpc/proto/rpc_extensions.pb.h"
#include "google/protobuf/descriptor.h"

ARPC_BEGIN_NAMESPACE(arpc)

struct RpcReqArg {
    RpcReqArg(ANetRPCController *controller, RPCMessage *response, RPCClosure *closure, CodecContext *context) {
        sController = controller;
        sResponse = response;
        sClosure = closure;
        sContext = context;
        sVersion = ARPC_VERSION_CURRENT;
    }

    ~RpcReqArg() { delete sContext; }
    ANetRPCController *sController;
    RPCMessage *sResponse;
    RPCClosure *sClosure;
    CodecContext *sContext;
    version_t sVersion;
};

class PacketCodeBuilder {
public:
    uint32_t operator()(const RPCMethodDescriptor *method) {
        const RPCServiceDescriptor *pSerDes = method->service();

        uint32_t serviceId = (uint32_t)(pSerDes->options().GetExtension(global_service_id));
        uint32_t methodId = (uint32_t)(method->options().GetExtension(local_method_id));

        uint32_t ret = (serviceId << 16) | methodId;

        return ret;
    }
};

class PacketCodeParser {
public:
    void operator()(uint32_t pCode, uint16_t &serviceId, uint16_t &methodId) {
        serviceId = 0;
        methodId = 0;
        serviceId = pCode >> 16;
        methodId = pCode & (0xFFFF);
        return;
    }
};

ARPC_END_NAMESPACE(arpc)
#endif // ARPC_PACKETARG_H
