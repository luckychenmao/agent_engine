#ifndef ARPC_ADMIN_SERVICE_H
#define ARPC_ADMIN_SERVICE_H
#include <string>

#include "agent_engine/network/anet/adminserver.h"
#include "agent_engine/network/anet/anet.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"

ARPC_BEGIN_NAMESPACE(arpc);

/**
 * The interface class for ARPC applications to enable admin service.
 * There are two cases:
 * 1. You just want to enable anet/krpc debugging interface
 * 2. You want to extend the admin service with your own subsystem
 *
 * For case 1, it is very simple:
 * @code
 *
 * #include "agent_engine/network/arpc/arpc/AdminService.h"
 * ...
 * AdminService::Start();
 * ...
 *
 * //Before exit
 * AdminService::Stop();
 *
 * @endcode
 *
 * For case 2, some extra steps:
 *
 * @code
 *
 *   anet::AdminServer * admServer = AdminService::GetAdminInstance();
 *
 *   string subSys = "arpc";
 *   SubSysEntry *arpcSys = admServer->registerSubSys(subSys);
 *   arpcSys->usage = "arpc";
 *
 *   string cmdName = "dump";
 *   string usage = "dump [rpcservers|all]\n"
 *       "    rpcservers: dump all rpcservers information\n"
 *       "    all: default value. all of above.\n";
 *   admServer->registerCmd(subSys, cmdName, usage, dump);
 *
 * @endcode
 */

class AdminService {
public:
    /**
     * Register krpc/anet admin commands and start admin service.
     * This function may allocate the AdminServer instance.
     */
    static void Start();

    /**
     * Stop admin service and free the allocated AdminServer instance.
     */
    static bool Stop();

    /**
     * Get admin server instance pointer. User only need to get the
     * instance when trying to register new sub-system or commands.
     */
    static anet::AdminServer *GetAdminInstance(std::string spec = "");

private:
    static anet::AdminServer *adm;
};

ARPC_END_NAMESPACE(arpc);

#endif
