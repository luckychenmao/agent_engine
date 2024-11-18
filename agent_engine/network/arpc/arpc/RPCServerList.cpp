#include "agent_engine/network/arpc/arpc/RPCServerList.h"

#include <algorithm>
#include <cassert>
#include <ostream>
#include <stddef.h>

#include "agent_engine/network/anet/ilogger.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/RPCServer.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"

using namespace std;

ARPC_BEGIN_NAMESPACE(arpc);

static int RPCServerCount = 0;

RPCServerListT &getRPCServerList() {
    static RPCServerListT RPCServerList;
    return RPCServerList;
}

std::mutex &getRPCServerListMutex() {
    static std::mutex RPCServerListMutex;
    return RPCServerListMutex;
}

int getRPCServerCount() {
    std::lock_guard lock(getRPCServerListMutex());
    return RPCServerCount;
}

bool addRPCServerToList(RPCServer *s) {
    bool added = false;
    int i = 0;
    RPCServerListT &list = getRPCServerList();
    std::mutex &mutex = getRPCServerListMutex();

    if (findRPCServerFromList(s) < 0) {
        {
            std::lock_guard lock(mutex);

            for (; i < MAX_RPCSERVERS; ++i) {
                if (list[i] == NULL) {
                    list[i] = s;
                    RPCServerCount++;
                    assert(RPCServerCount <= MAX_RPCSERVERS);
                    added = true;
                    break;
                }
            }
        }

        if (i == MAX_RPCSERVERS) {
            ARPC_LOG(WARN, "Slot number limits to %d. No more slots for RPCServer: %p", MAX_RPCSERVERS, s);
        }
    }

    return added;
}

bool delRPCServerFromList(RPCServer *s) {
    bool deleted = false;
    RPCServerListT &list = getRPCServerList();
    std::mutex &mutex = getRPCServerListMutex();

    int i = findRPCServerFromList(s);
    std::lock_guard lock(mutex);

    if (i >= 0 && i < MAX_RPCSERVERS) {
        list[i] = NULL;
        RPCServerCount--;
        assert(RPCServerCount >= 0);
        deleted = true;
    }

    return deleted;
}

int findRPCServerFromList(RPCServer *s) {
    int rc = -1;
    RPCServerListT &list = getRPCServerList();
    std::mutex &mutex = getRPCServerListMutex();

    std::lock_guard lock(mutex);

    for (int i = 0; i < MAX_RPCSERVERS; ++i) {
        if (list[i] == s) {
            rc = i;
            break;
        }
    }

    return rc;
}

int dumpRPCServerList(ostringstream &out) {
    RPCServerListT &list = getRPCServerList();
    std::mutex &mutex = getRPCServerListMutex();

    int count = getRPCServerCount();
    out << "===============================Dump of RPCServers===============================" << endl
        << "Total count of RPCServers: " << count << endl;

    if (count == MAX_RPCSERVERS) {
        out << "[Warning]: RPCServer count is up to limit(" << MAX_RPCSERVERS << ")." << endl
            << "           Dumped information maybe incomplete." << endl;
    }

    std::lock_guard lock(mutex);

    for (int i = 0; i < MAX_RPCSERVERS; ++i) {
        if (list[i] != NULL)
            list[i]->dump(out);
    }

    return 0;
}

ARPC_END_NAMESPACE(arpc);
