#ifndef ANET_DIRECT_PACKET_STREAMER_H_
#define ANET_DIRECT_PACKET_STREAMER_H_

#include <stdint.h>

#include "agent_engine/network/anet/defaultpacketstreamer.h"
#include "agent_engine/network/anet/ipacketfactory.h"

namespace anet {
class ChannelPool;
class DataBuffer;
class DirectPacket;
class DirectPayload;
class DirectPlaceholder;
class DirectStreamingContext;
class PacketHeader;
class Socket;
class StreamingContext;
struct DirectPacketHeader;

constexpr int64_t DIRECT_WRITE_PAYLOAD_THRESHOLD = 8 * 1024;

class Packet;

class DirectPacketFactory : public IPacketFactory {
public:
    ~DirectPacketFactory() override {}
    Packet *createPacket(int pcode) override;
    DirectPacket *createDirectPacket(int pcode);
};

class DirectPacketStreamer : public DefaultPacketStreamer {
    friend class DirectPacketStreamerTest_testGetPacketInfo_Test;
    friend class DirectPacketStreamerTest_testGetPlaceholderFromChannel_Test;

public:
    DirectPacketStreamer();
    ~DirectPacketStreamer();
    StreamingContext *createContext() override;

    using DefaultPacketStreamer::encode;
    bool encode(Packet *packet, DataBuffer *output, DirectPayload &payloadToWrite);
    bool processData(DataBuffer *dataBuffer, StreamingContext *context) override;
    bool readPayload(DataBuffer *input,
                     Socket *socket,
                     ChannelPool *channelPool,
                     DirectStreamingContext *context,
                     int &socketReadLen);

private:
    using DefaultPacketStreamer::getPacketInfo;
    bool getPacketInfo(DataBuffer *input, PacketHeader *header, DirectPacketHeader *directHeader, bool *broken);
    const DirectPlaceholder &getPlaceholderFromChannel(ChannelPool *channelPool, uint64_t channelId);
    Packet *decode(DataBuffer *input, PacketHeader *header) override;

private:
    DirectPacketFactory _directPacketFactory;
};

} // namespace anet

#endif // ANET_DIRECT_PACKET_STREAMER_H_
