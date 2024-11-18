#include "agent_engine/network/anet/adminclient.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agent_engine/network/anet/anet.h"
#include "agent_engine/network/anet/connection.h"
#include "agent_engine/network/anet/ilogger.h"
#include "agent_engine/network/anet/log.h"
#include "agent_engine/network/anet/transport.h"

namespace anet {
class IPacketHandler;
} // namespace anet

BEGIN_ANET_NS();

AdminClient::AdminClient(Transport *trans, char *spec) : _streamer(&_factory) {
    _spec = strdup(spec);
    _transport = trans;
}

AdminClient::~AdminClient() {
    if (_spec) {
        free(_spec);
    }
}

int AdminClient::start(IPacketHandler *handler, void *args, char *cmd, int paramCount, char **params) {
    Connection *conn;
    conn = _transport->connect(_spec, &_streamer, true);
    if (conn == NULL) {
        ANET_LOG(ERROR, "connection error.");
        return -1;
    }
    conn->setDefaultPacketHandler(handler);
    conn->setQueueLimit(5u);

    _transport->start();
    char buffer[MAX_CMD_BUF];

    /* Now build the admin command buffer.
     * The cmd buffer will contain the following pattern:
     * <cmd str> <param 1> <param 2> ... <param n>
     */
    char paraStr[MAX_CMD_BUF];
    int offset = 0;
    if (paramCount > 0) {
        for (int i = 0; i < paramCount; i++) {
            offset += snprintf(paraStr + offset, MAX_CMD_BUF - offset, " %s", params[i]);
        }
        paraStr[MAX_CMD_BUF - 1] = '\0';
    } else if (paramCount == 0) {
        paraStr[0] = '\0';
    }
    snprintf(buffer, MAX_CMD_BUF, "%s %s", cmd, paraStr);
    buffer[MAX_CMD_BUF - 1] = '\0';

    AdvanceDefaultPacket *packet = new AdvanceDefaultPacket();
    /* The body should include \0 */
    packet->setBody(buffer, strlen(buffer) + 1);
    ANET_LOG(DEBUG, "About to post command to server. \n cmd: %s", buffer);
    if (!(conn->postPacket(packet, handler, args, true))) {
        ANET_LOG(SPAM, "postPacket() Failed! IOC(%p)", conn);
        conn->subRef();
        delete packet;
        return -1;
    }

    ANET_LOG(INFO, "post finish.");
    conn->subRef();

    return 0;
}

END_ANET_NS();
