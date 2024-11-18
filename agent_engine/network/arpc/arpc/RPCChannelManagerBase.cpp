#include "agent_engine/network/arpc/arpc/RPCChannelManagerBase.h"

#include <cstddef>

#include "agent_engine/network/anet/connection.h"
#include "agent_engine/network/anet/ilogger.h"
#include "agent_engine/network/anet/runnable.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"

using namespace std;
using namespace anet;
ARPC_BEGIN_NAMESPACE(arpc);
ARPC_DECLARE_AND_SETUP_LOGGER(RPCChannelManagerBase);

RPCChannelManagerBase::RPCChannelManagerBase() {}

RPCChannelManagerBase::~RPCChannelManagerBase() {}

void RPCChannelManagerBase::Close() {}

ARPC_END_NAMESPACE(arpc);
