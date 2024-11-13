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
#include "agent_engine/network/arpc/arpc/RPCServerClosure.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "agent_engine/network/anet/connection.h"
#include "agent_engine/network/anet/ilogger.h"
#include "agent_engine/network/anet/packet.h"
#include "agent_engine/network/anet/runnable.h"
#include "agent_engine/network/arpc/arpc/MessageCodec.h"
#include "agent_engine/network/arpc/arpc/MessageSerializable.h"
#include "agent_engine/network/arpc/arpc/Tracer.h"
#include "agent_engine/network/arpc/arpc/UtilFun.h"
#include "agent_engine/network/arpc/arpc/proto/rpc_extensions.pb.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"

using namespace anet;
ARPC_BEGIN_NAMESPACE(arpc);
ARPC_DECLARE_AND_SETUP_LOGGER(RPCServerClosure);

RPCServerClosure::RPCServerClosure(RPCMessage *reqMsg, RPCMessage *resMsg)
    : _reqMsg(reqMsg), _resMsg(resMsg), _tracer(nullptr) {}

RPCServerClosure::~RPCServerClosure() {
    _reqMsg = NULL;
    if (_tracer) {
        delete _tracer;
        _tracer = NULL;
    }
}

void RPCServerClosure::Run() {
    if (_tracer) {
        int32_t timeout = _tracer->GetClientTimeout();
        _tracer->BeginPostResponse();
        if (timeout > 0 &&
            _tracer->GetBeginPostResponse() - _tracer->GetBeginHandleRequest() > (int64_t)timeout * 1000) {
            ARPC_LOG(ERROR,
                     "Service Timeout: %s request recv: %ld, request process start: %ld, request process end: %ld, "
                     "timeout: %d(ms)",
                     getIpAndPortAddr().c_str(),
                     _tracer->GetBeginHandleRequest(),
                     _tracer->GetBeginWorkItemProcess(),
                     _tracer->GetBeginPostResponse(),
                     _tracer->GetClientTimeout());
        }
    }
    doPostPacket();
    if (_tracer) {
        _tracer->EndHandleRequest(ARPC_ERROR_NONE);
    }
    _arena.reset();
    release();
}

ARPC_END_NAMESPACE(arpc);
