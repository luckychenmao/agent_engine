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
/**
 * File name: delaydecodepacketfactory.cpp
 * Author: lizhang
 * Create time: 2010-11-07 18:38:34
 * $Id$
 *
 * Description: ***add description here***
 *
 */

#include "agent_engine/network/anet/delaydecodepacketfactory.h"

#include "agent_engine/network/anet/delaydecodepacket.h"

namespace anet {
class Packet;

DelayDecodePacketFactory::DelayDecodePacketFactory() {}

DelayDecodePacketFactory::~DelayDecodePacketFactory() {}

Packet *DelayDecodePacketFactory::createPacket(int /*pcode*/) { return new DelayDecodePacket; }

} /*end namespace anet*/