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
#ifndef ANET_ANET_H
#define ANET_ANET_H
#include "agent_engine/network/anet/advancepacket.h"
#include "agent_engine/network/anet/advancepacketfactory.h"
#include "agent_engine/network/anet/connection.h"
#include "agent_engine/network/anet/connectionpriority.h"
#include "agent_engine/network/anet/controlpacket.h"
#include "agent_engine/network/anet/databuffer.h"
#include "agent_engine/network/anet/databufferserializable.h"
#include "agent_engine/network/anet/defaultpacket.h"
#include "agent_engine/network/anet/defaultpacketfactory.h"
#include "agent_engine/network/anet/defaultpacketstreamer.h"
#include "agent_engine/network/anet/delaydecodepacket.h"
#include "agent_engine/network/anet/delaydecodepacketfactory.h"
#include "agent_engine/network/anet/httppacket.h"
#include "agent_engine/network/anet/httppacketfactory.h"
#include "agent_engine/network/anet/httpstreamer.h"
#include "agent_engine/network/anet/iocomponent.h"
#include "agent_engine/network/anet/ipacketfactory.h"
#include "agent_engine/network/anet/ipackethandler.h"
#include "agent_engine/network/anet/ipacketstreamer.h"
#include "agent_engine/network/anet/iserveradapter.h"
#include "agent_engine/network/anet/packet.h"
#include "agent_engine/network/anet/streamingcontext.h"
#include "agent_engine/network/anet/timeutil.h"
#include "agent_engine/network/anet/transport.h"

// DO NOT export interfaces about logging implicitly
//#include "agent_engine/network/anet/log.h"

/**legacy http related header files*/
#include "agent_engine/network/anet/httppacketstreamer.h"
#include "agent_engine/network/anet/httprequestpacket.h"
#include "agent_engine/network/anet/httpresponsepacket.h"

// for direct packet
#include "agent_engine/network/anet/directpacket.h"
#include "agent_engine/network/anet/directpacketstreamer.h"
#include "agent_engine/network/anet/directstreamingcontext.h"
#include "agent_engine/network/anet/directtcpconnection.h"

#define BEGIN_ANET_NS() namespace anet {
#define END_ANET_NS() }
#define USE_ANET_NS() using namespace anet;

BEGIN_ANET_NS()

/* Redefine advance packet type to make it easier to use. */
typedef AdvancePacket<DelayDecodePacket> AdvanceDelayDecodePacket;
typedef AdvancePacket<DefaultPacket> AdvanceDefaultPacket;
typedef AdvancePacketFactory<DelayDecodePacket> AdvanceDelayDecodePacketFactory;
typedef AdvancePacketFactory<DefaultPacket> AdvanceDefaultPacketFactory;

END_ANET_NS()

#define ANET_VERSION 040000
#endif /*End of ANET_ANET_H*/
