/**
 * File name: defaultpacketfactory.cpp
 * Author: zhangli
 * Create time: 2008-12-25 11:38:19
 * $Id$
 *
 * Description: ***add description here***
 *
 */

#include "agent_engine/network/anet/defaultpacketfactory.h"

#include "agent_engine/network/anet/defaultpacket.h"

namespace anet {
Packet *DefaultPacketFactory::createPacket(int pcode) { return new DefaultPacket; }

} /*end namespace anet*/
