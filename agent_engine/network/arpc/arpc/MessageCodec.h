#ifndef ARPC_MESSAGECODEC_H
#define ARPC_MESSAGECODEC_H
#include <assert.h>
#include <memory>
#include <stddef.h>
#include <stdint.h>
#include <string>

#include "agent_engine/network/anet/anet.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/Tracer.h"
#include "agent_engine/network/arpc/arpc/proto/rpc_extensions.pb.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"

ARPC_BEGIN_NAMESPACE(arpc);

struct CallId {
    std::string strId;
    uint32_t intId;
};

struct CodecContext {
    CodecContext() {
        request = NULL;
        rpcService = NULL;
        rpcMethodDes = NULL;
        errorCode = ARPC_ERROR_NONE;
        enableTrace = false;
        ownRequest = false;
        timeout = 0;
        qosId = 0;
        requestId = 0;
    }

    CallId callId;          // for encode
    RPCMessage *request;    // for en/decode
    std::string token;      // for en/decode
    RPCService *rpcService; // for decode
    std::shared_ptr<RPCService> rpcServicePtr;
    RPCMethodDescriptor *rpcMethodDes; // for decode
    ErrorCode errorCode;
    bool enableTrace;
    bool ownRequest;
    int32_t timeout;
    std::string clientName;
    uint32_t qosId;
    std::string userPayload;
    std::shared_ptr<google::protobuf::Arena> arena;
    uint32_t requestId;
};

class RPCServer;

class MessageCodec {
public:
    MessageCodec();
    virtual ~MessageCodec();

public:
    virtual CallId GenerateCallId(const RPCMethodDescriptor *method, version_t version) const;

protected:
    friend class MessageCodecTest;
};

TYPEDEF_PTR(MessageCodec);
ARPC_END_NAMESPACE(arpc);

#endif // ARPC_MESSAGECODEC_H
