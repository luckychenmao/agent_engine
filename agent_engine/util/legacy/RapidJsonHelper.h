
#pragma once

#include <algorithm>
#include <assert.h>
#include <limits>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <type_traits>
#include <vector>

#include "util/Log.h"
#include "util/MultiValueCreator.h"
#include "util/MultiValueFormatter.h"
#include "util/MultiValueType.h"
#include "util/MultiValueWriter.h"
#include "util/legacy/RapidJsonCommon.h"
#include "util/mem_pool/Pool.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace util {

template <typename T, typename = void>
struct IsStdVectorType : std::false_type {};

template <typename T>
struct IsStdVectorType<T, typename std::enable_if<std::is_same<T, std::vector<typename T::value_type>>::value>::type>
    : std::true_type {};

class RapidJsonHelper {
private:
    RapidJsonHelper(const RapidJsonHelper &);
    RapidJsonHelper &operator=(const RapidJsonHelper &);

public:
    static std::string SimpleValue2Str(const SimpleValue &value);

    template <typename T>
    static T getWithDefault(const SimpleDocument &doc, const std::string &key, const T &defaultValue = T());
    template <typename T>
    static T getWithDefault(const SimpleValue &doc, const std::string &key, const T &defaultValue = T());
    template <typename T>
    static T getValue(const SimpleValue &value);

    template <typename T>
    static std::vector<T> getVectorValue(const SimpleValue &value);

    template <typename T>
    static util::MultiValueType<T> getMultiValue(const SimpleValue &value, util::mem_pool::Pool *pool);

    template <typename T>
    static T invalidValue();

    template <typename T>
    static T getValue(const SimpleValue &value, util::mem_pool::Pool *pool);

private:
    template <typename T>
    static T getValue(const SimpleValue &value, util::mem_pool::Pool *pool, std::true_type);

    template <typename T>
    static T getValue(const SimpleValue &value, util::mem_pool::Pool *pool, std::false_type);

    template <typename T>
    static T getValueMaybeVec(const SimpleValue &value, std::true_type) {
        return getVectorValue<typename T::value_type>(value);
    }

private:
    LOG_DECLARE();
};

template <>
inline double RapidJsonHelper::invalidValue<double>() {
    return std::numeric_limits<double>::quiet_NaN();
}

template <typename T>
inline T RapidJsonHelper::invalidValue() {
    return std::numeric_limits<T>::max();
}

template <typename T>
T RapidJsonHelper::getWithDefault(const SimpleDocument &doc, const std::string &key, const T &defaultValue) {
    if (!doc.IsObject()) {
        return defaultValue;
    }
    auto iter = doc.FindMember(key);
    if (iter == doc.MemberEnd()) {
        return defaultValue;
    }
    return getValue<T>(iter->value);
}

template <typename T>
T RapidJsonHelper::getWithDefault(const SimpleValue &doc, const std::string &key, const T &defaultValue) {
    if (!doc.IsObject()) {
        return defaultValue;
    }
    auto iter = doc.FindMember(key);
    if (iter == doc.MemberEnd()) {
        return defaultValue;
    }
    return getValue<T>(iter->value);
}

template <>
inline bool RapidJsonHelper::getValue<bool>(const SimpleValue &value) {
    if (!value.IsBool()) {
        LOG(DEBUG, "get bool value failed");
        return false;
    }
    return value.GetBool();
}

template <>
inline int8_t RapidJsonHelper::getValue<int8_t>(const SimpleValue &value) {
    if (!value.IsInt()) {
        LOG(DEBUG, "get int8 value failed");
        return invalidValue<int8_t>();
    }
    return (int8_t)value.GetInt();
}

template <>
inline uint8_t RapidJsonHelper::getValue<uint8_t>(const SimpleValue &value) {
    if (!value.IsUint()) {
        LOG(DEBUG, "get uint8 value failed");
        return invalidValue<uint8_t>();
    }
    return (uint8_t)value.GetUint();
}

template <>
inline int16_t RapidJsonHelper::getValue<int16_t>(const SimpleValue &value) {
    if (!value.IsInt()) {
        LOG(DEBUG, "get int16 value failed");
        return invalidValue<int16_t>();
    }
    return (int16_t)value.GetInt();
}

template <>
inline uint16_t RapidJsonHelper::getValue<uint16_t>(const SimpleValue &value) {
    if (!value.IsUint()) {
        LOG(DEBUG, "get uint16 value failed");
        return invalidValue<uint16_t>();
    }
    return (uint16_t)value.GetUint();
}

template <>
inline int32_t RapidJsonHelper::getValue<int32_t>(const SimpleValue &value) {
    if (!value.IsInt()) {
        LOG(DEBUG, "get int32 value failed");
        return invalidValue<int32_t>();
    }
    return value.GetInt();
}

template <>
inline uint32_t RapidJsonHelper::getValue<uint32_t>(const SimpleValue &value) {
    if (!value.IsUint()) {
        LOG(DEBUG, "get uint32 value failed");
        return invalidValue<uint32_t>();
    }
    return value.GetUint();
}

template <>
inline int64_t RapidJsonHelper::getValue<int64_t>(const SimpleValue &value) {
    if (!value.IsInt64()) {
        LOG(DEBUG, "get uint64 value failed");
        return invalidValue<int64_t>();
    }
    return value.GetInt64();
}

template <>
inline long long int RapidJsonHelper::getValue<long long int>(const SimpleValue &value) {
    if (!value.IsInt64()) {
        LOG(DEBUG, "get uint64 value failed");
        return invalidValue<int64_t>();
    }
    return value.GetInt64();
}

template <>
inline uint64_t RapidJsonHelper::getValue<uint64_t>(const SimpleValue &value) {
    if (!value.IsUint64()) {
        LOG(DEBUG, "get uint64 value failed");
        return invalidValue<uint64_t>();
    }
    return value.GetUint64();
}

template <>
inline double RapidJsonHelper::getValue<double>(const SimpleValue &value) {
    if (!value.IsDouble()) {
        LOG(DEBUG, "get double value failed");
        return invalidValue<double>();
    }
    return value.GetDouble();
}

template <>
inline float RapidJsonHelper::getValue<float>(const SimpleValue &value) {
    if (!value.IsDouble()) {
        LOG(DEBUG, "get float value failed");
        return (float)invalidValue<double>();
    }
    return (float)value.GetDouble();
}

template <>
inline std::string RapidJsonHelper::getValue<std::string>(const SimpleValue &value) {
    if (!value.IsString()) {
        LOG(DEBUG, "get string value failed");
        return "";
    }
    return std::string(value.GetString(), value.GetStringLength());
}

template <typename T>
T RapidJsonHelper::getValue(const SimpleValue &value) {
    return getValueMaybeVec<T>(value, typename IsStdVectorType<T>::type());
}

template <typename T>
inline std::vector<T> RapidJsonHelper::getVectorValue(const SimpleValue &value) {
    std::vector<T> values;
    if (value.IsArray()) {
        size_t size = value.Size();
        for (size_t i = 0; i < size; i++) {
            values.push_back(getValue<T>(value[i]));
        }
    }
    return values;
}

template <>
inline util::MultiString RapidJsonHelper::getMultiValue<util::MultiChar>(const SimpleValue &value,
                                                                           util::mem_pool::Pool *pool) {
    typedef util::MultiString MultiString;
    if (!value.IsArray()) {
        return MultiString();
    }
    std::vector<std::string> strVec;
    for (size_t i = 0; i < value.Size(); i++) {
        assert(value[i].IsString());
        strVec.push_back(getValue<std::string>(value[i]));
    }
    char *buffer = util::MultiValueCreator::createMultiStringBuffer(strVec, pool);
    MultiString ms;
    ms.init(buffer);
    return ms;
}

template <>
inline util::MultiChar RapidJsonHelper::getMultiValue<char>(const SimpleValue &value, util::mem_pool::Pool *pool) {
    typedef util::MultiChar MultiChar;
    std::string str = getValue<std::string>(value);
    char *buffer = util::MultiValueCreator::createMultiValueBuffer<char>((char *)str.data(), str.length(), pool);
    MultiChar mc;
    mc.init(buffer);
    return mc;
}

template <typename T>
inline util::MultiValueType<T> RapidJsonHelper::getMultiValue(const SimpleValue &value, util::mem_pool::Pool *pool) {
    typedef util::MultiValueType<T> MultiValueTyped;
    MultiValueTyped ret;
    if (!value.IsArray()) {
        return ret;
    }

    size_t size = value.Size();
    size_t bufLen = util::MultiValueFormatter::calculateBufferLen(size, sizeof(T));
    char *buffer = (char *)pool->allocate(bufLen);
    util::MultiValueWriter<T> mvWriter;
    bool initFlag = mvWriter.init(buffer, bufLen, size);
    assert(initFlag);
    (void)initFlag;
    for (size_t i = 0; i < value.Size(); i++) {
        mvWriter.set(i, getValue<T>(value[i]));
    }
    ret.init(buffer);
    return ret;
}

inline std::string RapidJsonHelper::SimpleValue2Str(const SimpleValue &value) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    value.Accept(writer);
    return buffer.GetString();
}

template <typename T>
T RapidJsonHelper::getValue(const SimpleValue &value, util::mem_pool::Pool *pool) {
    return getValue<T>(value, pool, typename util::IsMultiType<T>::type());
}

template <typename T>
T RapidJsonHelper::getValue(const SimpleValue &value, util::mem_pool::Pool *pool, std::true_type) {
    return getMultiValue<typename T::type>(value, pool);
}

template <typename T>
T RapidJsonHelper::getValue(const SimpleValue &value, util::mem_pool::Pool *pool, std::false_type) {
    return getValue<T>(value);
}

} // namespace util
