#ifndef ADMIN_CLIENT_H
#define ADMIN_CLIENT_H

#include "agent_engine/network/anet/anet.h"
#include "agent_engine/network/anet/defaultpacketstreamer.h"

namespace anet {
class IPacketHandler;
class Transport;
} // namespace anet

#define MAX_CMD_BUF 1024
namespace anet {

class AdminClient {
public:
    AdminClient(Transport *transport, char *spec);
    ~AdminClient();
    int start(IPacketHandler *handler, void *args, char *cmd, int paramCount, char **params);

private:
    char *_spec;
    Transport *_transport;
    AdvanceDefaultPacketFactory _factory;
    DefaultPacketStreamer _streamer;
};

} // namespace anet
#endif
