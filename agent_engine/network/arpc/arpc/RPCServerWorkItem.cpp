/*
 * Copyright 2014-present Alibaba Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "agent_engine/network/arpc/arpc/RPCServerWorkItem.h"

#include <iosfwd>
#include <string.h>
#include <string>

#include "agent_engine/network/anet/connection.h"
#include "agent_engine/network/anet/ilogger.h"
#include "agent_engine/network/anet/iocomponent.h"
#include "agent_engine/network/anet/runnable.h"
#include "agent_engine/network/anet/socket.h"
#include "agent_engine/network/arpc/arpc/CommonMacros.h"
#include "agent_engine/network/arpc/arpc/RPCServerAdapter.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"

using namespace std;
using namespace anet;
ARPC_BEGIN_NAMESPACE(arpc);
ARPC_DECLARE_AND_SETUP_LOGGER(RPCServerWorkItem);

RPCServerWorkItem::RPCServerWorkItem(RPCService *pService,
                                     RPCMethodDescriptor *pMethodDes,
                                     RPCMessage *pReqMsg,
                                     const std::string &userPayload,
                                     Tracer *tracer) {
    _pService = pService;
    _pMethodDes = pMethodDes;
    _pReqMsg = pReqMsg;
    _userPayload = userPayload;
    _tracer = tracer;
}

RPCServerWorkItem::~RPCServerWorkItem() {}

void RPCServerWorkItem::destroy() {
    if (_tracer) {
        DELETE_AND_SET_NULL_ANET(_tracer);
    }
    delete this;
}

void RPCServerWorkItem::drop() {
    if (_tracer) {
        DELETE_AND_SET_NULL_ANET(_tracer);
    }
    delete this;
}

RPCMessage *RPCServerWorkItem::getRequest() { return _pReqMsg; }

std::string RPCServerWorkItem::getMethodName() {
    if (_pMethodDes) {
        return _pMethodDes->full_name();
    }
    return "";
}

ARPC_END_NAMESPACE(arpc);
