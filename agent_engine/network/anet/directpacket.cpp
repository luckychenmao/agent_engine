#include "agent_engine/network/anet/directpacket.h"

#include "agent_engine/network/anet/databuffer.h"
#include "agent_engine/network/anet/ilogger.h"
#include "agent_engine/network/anet/log.h"

namespace anet {

bool DirectPacket::decode(DataBuffer *input, DirectPacketHeader *header) {
    if (input->getDataLen() < header->_msgSize) {
        ANET_LOG(ERROR,
                 "invalid input to decode with dataLen:[%d] and header's msgSize:[%d]",
                 input->getDataLen(),
                 header->_msgSize);
        return false;
    }
    bool rc = setBody(input->getData(), header->_msgSize);
    input->drainData(header->_msgSize);
    return rc;
}

} // namespace anet
