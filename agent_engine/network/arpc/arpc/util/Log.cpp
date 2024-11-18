#include "agent_engine/network/arpc/arpc/util/Log.h"

#include <string>

#include "agent_engine/network/anet/alogadapter.h"

ARPC_BEGIN_NAMESPACE(arpc);

AlogAdapter alogadapter(std::string("arpc"));
ILogger *arpclogger = &alogadapter;

ARPC_END_NAMESPACE(arpc);
