#ifndef ARPC_RPCSERVICELIST_H
#define ARPC_RPCSERVICELIST_H
#include <sstream>

#include "agent_engine/network/arpc/arpc/CommonMacros.h"

ARPC_BEGIN_NAMESPACE(arpc);

class RPCServer;

const int MAX_RPCSERVERS = 1024;

typedef RPCServer *RPCServerListT[MAX_RPCSERVERS];

RPCServerListT &getRPCServerList();
std::mutex &getRPCServerListMutex();

bool addRPCServerToList(RPCServer *);
bool delRPCServerFromList(RPCServer *);
int findRPCServerFromList(RPCServer *);
int getRPCServerCount(void);

int dumpRPCServerList(std::ostringstream &out);

ARPC_END_NAMESPACE(arpc);

#endif // ARPC_RPCSERVICELIST_H
