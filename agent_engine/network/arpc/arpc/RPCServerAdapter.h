#ifndef ARPC_RPC_SERVER_ADAPTER_H
#define ARPC_RPC_SERVER_ADAPTER_H
#include <unordered_set>

#include "agent_engine/network/anet/anet.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/proto/rpc_extensions.pb.h"

ARPC_BEGIN_NAMESPACE(arpc);

class RPCServer;
class RPCServerWorkItem;
struct CodecContext;
class Tracer;

class RPCServerAdapter {
public:
    RPCServerAdapter(RPCServer *pRpcServer);

    virtual ~RPCServerAdapter();

public:
    int getClientConnectionNum() { return atomic_read(&_clientConnNum); }

protected:
    ErrorCode doPushWorkItem(RPCServerWorkItem *pWorkItem, CodecContext *pContext, Tracer *tracer);

private:
    friend class RPCServerAdapterTest;

protected:
    RPCServer *_pRpcServer;
    atomic_t _clientConnNum;
    pthread_rwlock_t _lock;
    bool _skipThreadPool{false};
};

ARPC_END_NAMESPACE(arpc);

#endif // ARPC_RPC_SERVER_ADAPTER_H
