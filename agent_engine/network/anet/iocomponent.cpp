#include "agent_engine/network/anet/iocomponent.h"

#include <assert.h>
#include <stddef.h>

#include "agent_engine/network/anet/atomic.h"
#include "agent_engine/network/anet/connection.h"
#include "agent_engine/network/anet/debug.h"
#include "agent_engine/network/anet/ilogger.h"
#include "agent_engine/network/anet/ioworker.h"
#include "agent_engine/network/anet/log.h"
#include "agent_engine/network/anet/socket.h"
#include "agent_engine/network/anet/socketevent.h"
#include "agent_engine/network/anet/threadmutex.h"
#include "agent_engine/network/anet/timeutil.h"
#include "agent_engine/network/anet/transport.h"

namespace anet {

/*
 * ����芥�
 */
IOComponent::IOComponent(Transport *owner, Socket *socket) {
    assert(socket);
    assert(owner);
    _owner = owner;
    _socket = socket;
    _socket->setIOComponent(this);
    _socketEvent = NULL;
    atomic_set(&_refcount, 1);
    _state = ANET_TO_BE_CONNECTING; // 姝ｅ�杩��
    _autoReconn = 0;                // 涓��������
    _prev = _next = NULL;
    _lastUseTime = TimeUtil::getTime();
    _enableRead = true;
    _enableWrite = false;
    _refByRreadWriteThread = false;
    _maxIdleTimeInMicroseconds = MAX_IDLE_TIME_IN_MICROSECONDS;
    _isSocketInEpoll = false;
    _type = IOC_BASE;
    _belongedWorker = NULL;
}

/*
 * ����芥�
 */
IOComponent::~IOComponent() {
    if (_socket) {
        _socket->close();
        delete _socket;
        _socket = NULL;
    }
}

void IOComponent::setSocketEvent(SocketEvent *socketEvent) { _socketEvent = socketEvent; }

void IOComponent::enableRead(bool on) {
    _enableRead = on;
    bool read = _enableRead;
    bool write = _enableWrite;
    if (_socketEvent) {
        ANET_LOG(SPAM, "setEvent(R:%d,W:%d). IOC(%p)", read, write, this);
        MutexGuard lock(&_socketMutex);
        if (!_isSocketInEpoll)
            return;
        bool rc = _socketEvent->setEvent(_socket, read, write);
        if (!rc)
            ANET_LOG(ERROR, "setEvent(R:%d,W:%d). IOC(%p) fail", read, write, this);
    }
}

void IOComponent::enableWrite(bool on) {
    _enableWrite = on;
    bool read = _enableRead;
    bool write = _enableWrite;
    if (_socketEvent) {
        ANET_LOG(SPAM, "setEvent(R:%d,W:%d). IOC(%p)", read, write, this);
        MutexGuard lock(&_socketMutex);
        if (!_isSocketInEpoll)
            return;
        bool rc = _socketEvent->setEvent(_socket, read, write);
        if (!rc)
            ANET_LOG(ERROR, "setEvent(R:%d,W:%d). IOC(%p) fail", read, write, this);
    }
}

void IOComponent::addRef() {
    ANET_LOG(SPAM, "IOC(%p)->addRef(), [%d]", this, _refcount.counter);
    atomic_add(1, &_refcount);
}

void IOComponent::subRef() {
    ANET_LOG(SPAM, "IOC(%p)->subRef(), [%d]", this, _refcount.counter);
    int ref = atomic_dec_return(&_refcount);
    if (!ref) {
        ANET_LOG(SPAM, "Deleting this IOC(%p)", this);
        delete this;
    }
}

void IOComponent::closeConnection(Connection *conn) { conn->closeHook(); }

int IOComponent::getRef() { return atomic_read(&_refcount); }

void IOComponent::lock() { _mutex.lock(); }

void IOComponent::unlock() { _mutex.unlock(); }

bool IOComponent::setState(IOCState state) {
    _state = state;
    return true;
}

void IOComponent::closeSocketNoLock() {
    if (_socket) {
        {
            MutexGuard lock(&_socketMutex);
            if (!_isSocketInEpoll)
                return;
            bool rc = _socketEvent->removeEvent(_socket);
            if (!rc) {
                ANET_LOG(ERROR, "IOC(%p)Remove event error", this);
            }
            _isSocketInEpoll = false;
        }
        _socket->close();
    }
}

void IOComponent::closeSocket() {
    lock();
    closeSocketNoLock();
    unlock();
}

RECONNErr IOComponent::reconnect() {
    /* we should never arrive here for now. */
    DBGASSERT(false);
    return ANET_RECONN_OK;
}

char *IOComponent::getLocalAddrStr(char *dest, int len) { return _socket->getAddr(dest, len, true); }

void IOComponent::initialize() {
    _belongedWorker = _owner->getBelongedWorker(this);
    _socketEvent = _belongedWorker->getSocketEvent();
    assert(_socketEvent);
}

void IOComponent::shutdownSocket() {
    if (_socket) {
        _socket->shutdown();
    }
}

} // namespace anet
