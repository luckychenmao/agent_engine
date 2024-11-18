#ifndef ANET_ISERVERADAPTER_H
#define ANET_ISERVERADAPTER_H
#include "agent_engine/network/anet/connection.h"
#include "agent_engine/network/anet/ipackethandler.h"
#include "agent_engine/network/anet/packet.h"

namespace anet {

class IServerAdapter {
public:
    /**
     * This function is used in Server. When ANET receive request,
     * it will call handlePacket() to deal with request packet.
     * This function is Called in Connection::handlePacket().
     * In some exceptions, ControlPacket(such as TimeoutPacket,
     * BadPacket) will be received. Obviously, you should consider
     * all these exceptions in you implementation.
     *
     * @param connection indicate where the request packet comes
     * from, you can use this connection to send a response.
     * @param packet request packet or ControlPacket
     * @return not used temporary.
     */
    virtual IPacketHandler::HPRetCode handlePacket(Connection *connection, Packet *packet) = 0;

    virtual ~IServerAdapter() {}
};
} // namespace anet

#endif /*ISERVERADAPTER_H*/
