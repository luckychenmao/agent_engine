#ifndef ANET_ADVANCETESTPACKETFACTORY_H_
#define ANET_ADVANCETESTPACKETFACTORY_H_

#include "agent_engine/network/anet/advancepacket.h"
#include "agent_engine/network/anet/advancetestpacket.h"
#include "agent_engine/network/anet/anet.h"
#include "agent_engine/network/anet/ipacketfactory.h"

BEGIN_ANET_NS();
template <class T>
class AdvanceTestPacketFactory : public IPacketFactory {
public:
    AdvanceTestPacketFactory();

    ~AdvanceTestPacketFactory();

    virtual Packet *createPacket(int pcode);
};

/*****************************************************************************/
/*****************************************************************************/
template <class T>
AdvanceTestPacketFactory<T>::AdvanceTestPacketFactory() {}

template <class T>
AdvanceTestPacketFactory<T>::~AdvanceTestPacketFactory() {}

template <class T>
Packet *AdvanceTestPacketFactory<T>::createPacket(int pcode) {
    /* We assume that no method id can be greater than 32768 for now.
     * Below is a bad assertion since server will treat pcode as
     * service id + method id, which is likely to trigger this assertion.*/
    // DBGASSERT(pcode >= 0);

    /* When pcode == 0, it normally means that this comes form the path for
     * client to allocate the packet and try to initiate a RPC. In this case
     * We should create AdvancePacket. */
    if (AdvancePacket<T>::IsAdvancePacket(pcode))
        return new AdvanceTestPacket<T>;
    else
        return new T;
}

END_ANET_NS();

#endif
