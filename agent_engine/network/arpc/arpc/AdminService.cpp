#include "agent_engine/network/arpc/arpc/AdminService.h"

#include <sstream>
#include <string.h>
#include <string>

#include "agent_engine/network/anet/admincmds.h"
#include "agent_engine/network/anet/adminserver.h"
#include "agent_engine/network/anet/ilogger.h"
#include "agent_engine/network/anet/runnable.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/RPCServerList.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"

using namespace std;
using namespace anet;

ARPC_BEGIN_NAMESPACE(arpc);

anet::AdminServer *AdminService::adm = NULL;
static std::mutex mAdminServiceMutex;
volatile static bool isAdminServiceStart = false;

static int dump(char *params, ostringstream &out);
/* just for test */
static int test(char *params, ostringstream &out);

void AdminService::Start() {
    std::lock_guard lock(mAdminServiceMutex);

    if (isAdminServiceStart)
        return;

    anet::AdminServer *admServer = GetAdminInstance();

    string subSys = "arpc";
    SubSysEntry *arpcSys = admServer->registerSubSys(subSys);
    arpcSys->usage = "arpc";

    string cmdName = "dump";
    string usage = "dump [rpcservers|all]\n"
                   "    rpcservers: dump all rpcservers information\n"
                   "    all: default value. all of above.\n";
    admServer->registerCmd(subSys, cmdName, usage, dump);

    cmdName = "test";
    usage = "test [echo <blabla>|fail]\n"
            "    echo: echo <blabla>\n"
            "    fail: default value. set pcode to E_FAILED.\n";
    admServer->registerCmd(subSys, cmdName, usage, test);

    admServer->runInThread();
    isAdminServiceStart = true;
}

bool AdminService::Stop() {
    bool rc = false;
    std::lock_guard lock(mAdminServiceMutex);

    if (adm && isAdminServiceStart) {
        adm->stop();
        delete adm;
        adm = NULL;
        rc = true;
        isAdminServiceStart = false;
    }

    return rc;
}

anet::AdminServer *AdminService::GetAdminInstance(std::string spec) {
    if (adm == NULL)
        adm = new anet::AdminServer(spec);

    return adm;
}

static int dump(char *params, ostringstream &out) {
    int rc = 0;
    bool dumpall = false;
    /* If no params are available, params will be set to NULL. */
    const char *p = params;

    /* If no parameter, assume "all" */
    if (p == NULL || *p == '\0')
        p = "all";

    if (strncmp(p, "all", 3) == 0)
        dumpall = true;

    if (strncmp(p, "rpcservers", 10) == 0 || dumpall) {
        rc = dumpRPCServerList(out);
        out << "--------------------------------------------------------------------------------" << endl;
    }

    ARPC_LOG(DEBUG, "AdminService: dump cmd executed, params: %s", params);
    return rc;
}

static int test(char *params, ostringstream &out) {
    int rc = -1;
    const char *p = params;

    if (p == NULL || *p == '\0')
        p = "fail";

    if (strncmp(p, "echo", 4) == 0) {
        const char *blabla = p + 4;

        // strip leading spaces
        while (*blabla != '\0' && *blabla == ' ')
            blabla++;

        out << blabla << endl;
        rc = 0;
    } else if (strncmp(p, "fail", 4) == 0) {
        // nothing to do
    }

    ARPC_LOG(DEBUG, "AdminService: test cmd executed, params: %s", params);
    return rc;
}

ARPC_END_NAMESPACE(arpc);
