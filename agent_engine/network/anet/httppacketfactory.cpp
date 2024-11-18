/**
 * File name: httppacketfactory.cpp
 * Author: zhangli
 * Create time: 2008-12-19 16:36:39
 * $Id$
 *
 * Description: ***add description here***
 *
 */

#include "agent_engine/network/anet/httppacketfactory.h"

#include "agent_engine/network/anet/httppacket.h"

namespace anet {
class Packet;

HTTPPacketFactory::HTTPPacketFactory() {}

Packet *HTTPPacketFactory::createPacket(int pcode) { return new HTTPPacket; }

} /*end namespace anet*/
