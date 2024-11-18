#ifndef ANET_UDPCONNECTION_H_
#define ANET_UDPCONNECTION_H_
#include "agent_engine/network/anet/connection.h"

namespace anet {
class IPacketStreamer;
class IServerAdapter;
class Socket;

class UDPConnection : public Connection {
    /*
     * 构造函数
     */
    UDPConnection(Socket *socket, IPacketStreamer *streamer, IServerAdapter *serverAdapter);

    /*
     * 析造函数
     */
    ~UDPConnection();

    /*
     * 写出数据
     *
     * @return 是否成功
     */
    bool writeData();

    /*
     * 读入数据
     *
     * @return 读入数据
     */
    bool readData();
};

} // namespace anet

#endif /*UDPCONNECTION_H_*/
