/**
 * File name: httppacketfactory.h
 * Author: zhangli
 * Create time: 2008-12-19 16:36:39
 * $Id$
 *
 * Description: ***add description here***
 *
 */

#ifndef ANET_HTTPPACKETFACTORY_H_
#define ANET_HTTPPACKETFACTORY_H_
#include "agent_engine/network/anet/ipacketfactory.h"

namespace anet {
class Packet;

class HTTPPacketFactory : public IPacketFactory {
public:
    HTTPPacketFactory();
    Packet *createPacket(int pcode);
};

} /*end namespace anet*/
#endif /*ANET_HTTPPACKETFACTORY_H_*/
