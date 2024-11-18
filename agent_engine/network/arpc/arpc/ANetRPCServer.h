#ifndef ARPC_ANET_RPC_SERVER_H__
#define ARPC_ANET_RPC_SERVER_H__
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <utility>

#include "agent_engine/network/anet/anet.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/RPCServer.h"
#include "agent_engine/network/arpc/arpc/anet/ANetApp.h"

ARPC_BEGIN_NAMESPACE(arpc);

class ANetRPCServer : public RPCServer {
public:
    /**
     * @param: transport instance for ANet.
     * If transport is NULL, ANetRPCServer will
     * create a private transport object.
     * Otherwise, ANetRPCServer will
     * share transport object with out world.
     */
    ANetRPCServer(anet::Transport *transport = NULL, size_t threadNum = 1, size_t queueSize = 50);
    /**
     * If there is a private transport instance,
     * it will be free.
     */
    virtual ~ANetRPCServer();

public:
    virtual bool Listen(const std::string &address,
                        int timeout = 5000,
                        int maxIdleTime = MAX_IDLE_TIME,
                        int backlog = LISTEN_BACKLOG) override;
    virtual bool Close() override;
    virtual void dump(std::ostringstream &out) override;
    virtual int GetListenPort() override;

public:
    bool OwnTransport() { return _anetApp.OwnTransport(); }

    /**
     * Make the private transport spin. IO thread and time thread will be activated.
     * @return bool, true if the transport is started successfully.
     */
    bool StartPrivateTransport() { return _anetApp.StartPrivateTransport(); }

    /**
     * Stop the private transport and destroy all the threads created.
     * This function must be called if StartPrivateTransport() is called.
     * @return true if the transport is stopped successfully.
     */
    bool StopPrivateTransport() { return _anetApp.StopPrivateTransport(); }
    anet::Transport *GetTransport() { return _anetApp.GetTransport(); }

    void SetMetricReporter(const std::shared_ptr<ServerMetricReporter> &metricReporter);

public:
    friend class ANetRPCServerTest;

private:
    ANetApp _anetApp;
    std::list<anet::IOComponent *> _ioComponentList;
    std::mutex _ioComponentMutex;
    std::shared_ptr<ServerMetricReporter> _metricReporter;
};

ARPC_END_NAMESPACE(arpc);

#endif
