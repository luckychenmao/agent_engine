#ifndef ANET_DIRECTTCPCONNECTION_H_
#define ANET_DIRECTTCPCONNECTION_H_

#include <stddef.h>

#include "agent_engine/network/anet/directpacket.h"
#include "agent_engine/network/anet/tcpconnection.h"

namespace anet {

class DirectPacketStreamer;
class DirectStreamingContext;
class IPacketStreamer;
class IServerAdapter;
class Socket;

class DirectTCPConnection : public TCPConnection {
public:
    DirectTCPConnection(Socket *socket, IPacketStreamer *streamer, IServerAdapter *serverAdapter);
    ~DirectTCPConnection();
    bool writeData() override;
    bool readData() override;

private:
    void clearWritingPacket();
    void finishPacketWrite();
    int sendBuffer(int &writeCnt, int &error);
    int sendPayload(int &writeCnt, int &error);

private:
    DirectPacketStreamer *_directStreamer;
    DirectStreamingContext *_directContext;
    size_t _payloadLeftToWrite = 0UL;
    DirectPayload _writingPayload;
    DirectPacket *_writingPacket = nullptr;
};

} // namespace anet

#endif // ANET_DIRECTTCPCONNECTION_H_
