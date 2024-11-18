#include "agent_engine/network/anet/e2eqos.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdint.h>

/**
 * 流控
 */

namespace anet {
#ifndef NULL
#define NULL 0
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////
E2eQos::E2eQos() {
    handle = NULL;
    _qos_quit_group = NULL;
    _qos_join_group = (int (*)(int, unsigned long, unsigned, unsigned))dlsym(RTLD_DEFAULT, "e2eqos_flow_setup");
    if (_qos_join_group != NULL)
        _qos_quit_group = (int (*)(int))dlsym(RTLD_DEFAULT, "e2eqos_flow_quit");
    else {
        handle = dlopen("libe2eqos.so", RTLD_LAZY);
        if (handle != NULL) {
            _qos_join_group = (int (*)(int, unsigned long, unsigned, unsigned))dlsym(handle, "e2eqos_flow_setup");
            _qos_quit_group = (int (*)(int))dlsym(handle, "e2eqos_flow_quit");
        }
    }
}

E2eQos::~E2eQos() {
    _qos_join_group = NULL;
    _qos_quit_group = NULL;
    if (handle != NULL)
        dlclose(handle);
}

int E2eQos::AddGroup(int sk, uint32_t gid, uint64_t jobid, uint32_t insid) {
    if (_qos_join_group == NULL) {
        return -1;
    }
    return (*_qos_join_group)(sk, jobid, insid, gid);
}

int E2eQos::RemoveGroup(int sk) {

    if (_qos_quit_group == NULL)
        return -1;
    return _qos_quit_group(sk);
}

E2eQos _ge2eQos;
} // namespace anet
