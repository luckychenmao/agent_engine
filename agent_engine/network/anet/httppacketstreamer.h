#ifndef ANET_HTTP_PACKET_STREAMER_H
#define ANET_HTTP_PACKET_STREAMER_H
#include "agent_engine/network/anet/defaultpacketstreamer.h"
#include "agent_engine/network/anet/httprequestpacket.h"
#include "agent_engine/network/anet/httpresponsepacket.h"
#include "agent_engine/network/anet/ipacketfactory.h"

namespace anet {

class DataBuffer;
class PacketHeader;

class HttpPacketStreamer : public DefaultPacketStreamer {
public:
    /*
     * 构造函数
     */
    HttpPacketStreamer(IPacketFactory *factory);
    /*
     * 数据包信息的设置
     */
    bool getPacketInfo(DataBuffer *input, PacketHeader *header, bool *broken);
};

/**
 * packet的factory, 缺省的httpd packet factory
 *
 * pcode = 1 是请求包
 * pcode = 0 是响应包
 */
class DefaultHttpPacketFactory : public IPacketFactory {
public:
    Packet *createPacket(int pcode) {
        if (pcode == 1) {
            return new HttpRequestPacket();
        } else {
            return new HttpResponsePacket();
        }
    }
};

} // namespace anet

#endif
