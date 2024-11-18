#ifndef ARPC_ANET_RPC_CONTROLLER_H
#define ARPC_ANET_RPC_CONTROLLER_H

#include <list>
#include <memory>
#include <string>

#include "agent_engine/network/anet/anet.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/Tracer.h"
#include "agent_engine/network/arpc/arpc/proto/rpc_extensions.pb.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"

ARPC_BEGIN_NAMESPACE(arpc);

class ANetRPCController : public RPCController {
public:
    ANetRPCController();
    ~ANetRPCController();

    virtual void Reset();
    virtual bool Failed() const;
    virtual std::string ErrorText() const;
    virtual void StartCancel();
    virtual void SetFailed(const std::string &reason);
    virtual bool IsCanceled() const;
    virtual void NotifyOnCancel(RPCClosure *callback);

    void SetErrorCode(ErrorCode errorCode);
    ErrorCode GetErrorCode();
    int GetExpireTime();
    /**
     * Set expire time. 0: use default(5s). < 0 never timeout.
     */
    void SetExpireTime(int milliseconds);
    void SetClientAddress(const std::string &clientAddress);
    std::string &GetClientAddress();
    void SetTraceFlag(bool flag);
    bool GetTraceFlag() const;
    Tracer &GetTracer();
    const Tracer &GetTracer() const;
    const TraceInfo *GetTraceInfo() const;
    void SetTraceInfo(TraceInfo *traceInfo);
    void PrintTraceInfo();

    void setProtoArena(const std::shared_ptr<google::protobuf::Arena> &arena);

private:
    std::shared_ptr<google::protobuf::Arena> _arena;
    bool _failed;
    std::string _reason;
    bool _canceled;
    std::list<RPCClosure *> _cancelList;
    ErrorCode _errorCode;
    int _expireTime;
    std::string _clientAddress;
    Tracer _tracer;
    TraceInfo *_traceInfo;
};

ARPC_END_NAMESPACE(arpc);
#endif // ARPC_ANET_RPC_CONTROLLER_H
