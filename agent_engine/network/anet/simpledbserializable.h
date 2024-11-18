/**
 * File name: simpledbserializable.h
 * Author: lizhang
 * Create time: 2010-11-07 09:54:40
 * $Id$
 *
 * Description: ***add description here***
 *
 */

#ifndef ANET_SIMPLEDBSERIALIZABLE_H_
#define ANET_SIMPLEDBSERIALIZABLE_H_

#include <iosfwd>

#include "agent_engine/network/anet/databufferserializable.h"

namespace anet {

template <typename T>
class SimpleDBSerializable : public DataBufferSerializable {
public:
    SimpleDBSerializable(const T &value = T()) : _value(value) {}
    SimpleDBSerializable(const SimpleDBSerializable<T> &from) : _value(from._value) {}
    ~SimpleDBSerializable() {}

    void set(const T &value) { _value = value; }

    const T &get() const { return _value; }

    virtual bool serialize(DataBuffer *output) const {
        output->write(_value);
        return true;
    }

    virtual bool deserialize(DataBuffer *input, int length = 0) {
        input->read(_value);
        return true;
    }

    bool operator==(const SimpleDBSerializable<T> &another) const { return _value == another._value; }

    bool operator<(const SimpleDBSerializable<T> &another) const { return _value < another._value; }

private:
    T _value;
};

template <typename T>
std::ostream &operator<<(std::ostream &out, const SimpleDBSerializable<T> &v) {
    return out << v.get();
}

} /*end namespace anet*/
#endif /*ANET_SIMPLEDBSERIALIZABLE_H_*/
