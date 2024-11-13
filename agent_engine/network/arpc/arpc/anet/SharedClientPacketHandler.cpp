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
#include "agent_engine/network/arpc/arpc/anet/SharedClientPacketHandler.h"

#include <stddef.h>

#include "agent_engine/network/anet/controlpacket.h"
#include "agent_engine/network/anet/ilogger.h"
#include "agent_engine/network/anet/packet.h"
#include "agent_engine/network/anet/runnable.h"
#include "agent_engine/network/arpc/arpc/ANetRPCChannel.h"
#include "agent_engine/network/arpc/arpc/ANetRPCController.h"
#include "agent_engine/network/arpc/arpc/MessageCodec.h"
#include "agent_engine/network/arpc/arpc/PacketArg.h"
#include "agent_engine/network/arpc/arpc/RPCChannelBase.h"
#include "agent_engine/network/arpc/arpc/Tracer.h"
#include "agent_engine/network/arpc/arpc/util/Log.h"

using namespace anet;

ARPC_BEGIN_NAMESPACE(arpc);
ARPC_DECLARE_AND_SETUP_LOGGER(SharedClientPacketHandler);

SharedClientPacketHandler::SharedClientPacketHandler() {
    _ref = 0;
    _func = NULL;
}

SharedClientPacketHandler::~SharedClientPacketHandler() {}

IPacketHandler::HPRetCode SharedClientPacketHandler::handlePacket(Packet *packet, void *args) {
    ARPC_LOG(DEBUG, "handle package in shared client package handler");
    if (_func != NULL) {
        (*_func)();
    }
    IPacketHandler::HPRetCode ret = doHandlePacket(packet, args);
    subRef();
    return ret;
}

IPacketHandler::HPRetCode SharedClientPacketHandler::doHandlePacket(Packet *packet, void *args) {
    RpcReqArg *pArgs = (RpcReqArg *)args;
    if (!packet->isRegularPacket()) {
        ARPC_LOG(WARN, "receive control packet[%d]", ((ControlPacket *)packet)->getCommand());
        return handleCmdPacket(packet, pArgs);
    }

    pArgs->sController->GetTracer().BeginHandleResponse();

    ARPC_LOG(TRACE1, "channel pointer: %p, msg chid: %lu", _channel, packet->getChannelId());
    {
        /* Check the error code and remote version.
         * Repost the request if necessary.
         */
        std::lock_guard lock(_channelLock);
        if (_channel && !_channel->CheckResponsePacket(packet, pArgs)) {
            return IPacketHandler::FREE_CHANNEL;
        }
    }

    decodePacket(pArgs->sController, packet, pArgs->sResponse, pArgs->sContext->arena);
    pArgs->sController->GetTracer().EndCallMethod(ARPC_ERROR_NONE);
    pArgs->sClosure->Run();
    delete pArgs;

    return IPacketHandler::FREE_CHANNEL;
}

void SharedClientPacketHandler::cleanChannel() {
    std::lock_guard lock(_channelLock);
    setChannel(NULL);
}

void SharedClientPacketHandler::addRef() {
    std::lock_guard lock(_mutex);
    _ref++;
    ARPC_LOG(DEBUG, "add ref, ref count:%ld.", _ref);
}

void SharedClientPacketHandler::subRef() {
    int64_t ref = 0;
    {
        std::lock_guard lock(_mutex);
        ref = --_ref;
    }

    ARPC_LOG(DEBUG, "sub ref, ref count:%ld.", ref);
    if (ref == 0) {
        delete this;
    }
}

void SharedClientPacketHandler::setHandlePacketPreFunc(const std::function<void()> *func) { _func = func; }

ARPC_END_NAMESPACE(arpc);
