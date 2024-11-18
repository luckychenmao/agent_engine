#ifndef ARPC_SYNCCLOSURE_H
#define ARPC_SYNCCLOSURE_H
#include "agent_engine/network/anet/anet.h"
#include "agent_engine/network/anet/threadcond.h"
#include "agent_engine/network/anet/threadmutex.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"

ARPC_BEGIN_NAMESPACE(arpc);

class SyncClosure : public RPCClosure {
public:
    SyncClosure() { _needWait = true; }
    virtual ~SyncClosure() {}

public:
    virtual void Run() {
        anet::MutexGuard guard(&_cond);
        _needWait = false;
        _cond.signal();
    }

    void WaitReply() {
        _cond.lock();

        while (_needWait) {
            _cond.wait();
        }

        _cond.unlock();
    }

private:
    anet::ThreadCond _cond;
    bool _needWait;
};

ARPC_END_NAMESPACE(arpc);

#endif // ARPC_SYNCCLOSURE_H
