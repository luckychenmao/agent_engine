
#pragma once

#include <map>
#include <ostream>
#include <stdint.h>
#include <string>
#include <utility>

#include "util/legacy/any.h"
#include "util/legacy/exception.h"
#include "util/legacy/fast_jsonizable_dec.h"
#include "util/legacy/json.h"
#include "util/legacy/jsonizable_exception.h"
#include "util/legacy/legacy_jsonizable.h"

namespace util {
namespace legacy {

class Jsonizable : public FastJsonizableBase {
public:
    static constexpr int FAST_MODE_MAGIC_NUMBER = 31415926;
    using FastJsonizableBase::Mode;

    class JsonWrapper : public FastJsonizable::JsonWrapper {
    public:
        JsonWrapper() : mFastMagicNum(0) {}

        JsonWrapper(const Any &json) : mFastMagicNum(0) {
            mMode = FROM_JSON;
            mJsonMap = AnyCast<std::map<std::string, Any>>(json);
        }

        JsonWrapper(RapidWriter *writer) : FastJsonizable::JsonWrapper(writer), mFastMagicNum(FAST_MODE_MAGIC_NUMBER) {}

        JsonWrapper(RapidValue *value) : FastJsonizable::JsonWrapper(value), mFastMagicNum(FAST_MODE_MAGIC_NUMBER) {}

        int32_t GetFastMagicNum() const { return mFastMagicNum; }

        std::map<std::string, Any> GetMap() const {
            if (mFastMagicNum == FAST_MODE_MAGIC_NUMBER) {
                std::map<std::string, Any> jsonMap;
                FromRapidValue(jsonMap, *getRapidValue());
                return jsonMap;
            } else {
                return mJsonMap;
            }
        }

        template <typename T>
        void Jsonize(const std::string &key, const T &value) {
            if (mFastMagicNum == FAST_MODE_MAGIC_NUMBER) {
                FastJsonizable::JsonWrapper::Jsonize(key, value);
            } else {
                AnyJsonize(key, value);
            }
        }

        template <typename T>
        void Jsonize(const std::string &key, T &value) {
            if (mFastMagicNum == FAST_MODE_MAGIC_NUMBER) {
                FastJsonizable::JsonWrapper::Jsonize(key, value);
            } else {
                AnyJsonize(key, value);
            }
        }

        template <typename T>
        void Jsonize(const std::string &key, T &value, const T &defaultValue) {
            if (mFastMagicNum == FAST_MODE_MAGIC_NUMBER) {
                FastJsonizable::JsonWrapper::Jsonize(key, value, defaultValue);
            } else {
                AnyJsonize(key, value, defaultValue);
            }
        }

        inline void JsonizeAsString(const std::string &key, std::string &value);
        inline void JsonizeAsString(const std::string &key, std::string &value, const std::string &defautValue);

        void Jsonize(const std::string &key, std::string &value, const std::string &defaultValue) {
            return Jsonize<std::string>(key, value, defaultValue);
        }

        void Jsonize(const std::string &key, int64_t &value, const int64_t &defaultValue) {
            return Jsonize<int64_t>(key, value, defaultValue);
        }

    private:
        template <typename T>
        void AnyJsonize(const std::string &key, const T &value);

        template <typename T>
        void AnyJsonize(const std::string &key, T &value);

        template <typename T>
        void AnyJsonize(const std::string &key, T &value, const T &defaultValue);

        void AnyJsonize(const std::string &key, RapidValue *&value) {
            UTIL_LEGACY_THROW(NotJsonizableException, "any jsonize do not support RapidValue, key: " + key);
        }
        void AnyJsonize(const std::string &key, RapidValue *&value, RapidValue *const &defaultValue) {
            UTIL_LEGACY_THROW(NotJsonizableException, "any jsonize do not support RapidValue, key: " + key);
        }

    private:
        std::map<std::string, Any> mJsonMap;
        // avoid wrong call, assume a lib is compiled by old util, that means
        // it's jsonwrapper do not have mFastMagicNum field, the access will get
        // an undefined value;
        int32_t mFastMagicNum;
    };

    virtual ~Jsonizable() {}
    virtual void Jsonize(JsonWrapper &json) = 0;
};

template <typename T>
void Jsonizable::JsonWrapper::AnyJsonize(const std::string &key, const T &value) {
    assert(mMode == TO_JSON);
    mJsonMap[key] = ToJson(value);
}

template <typename T>
void Jsonizable::JsonWrapper::AnyJsonize(const std::string &key, T &value) {
    if (mMode == TO_JSON)
        mJsonMap[key] = ToJson(value);
    else {
        std::map<std::string, Any>::const_iterator it = mJsonMap.find(key);
        if (it == mJsonMap.end()) {
            std::stringstream ss;
            ss << "context:{";
            for (std::map<std::string, Any>::const_iterator it = mJsonMap.begin(); it != mJsonMap.end(); it++) {
                ss << "," << it->first;
            }
            ss << "}";
            UTIL_LEGACY_THROW(NotJsonizableException, key + " not found when try to parse from Json." + ss.str());
        }
        FromJson(value, it->second);
    }
}

template <typename T>
void Jsonizable::JsonWrapper::AnyJsonize(const std::string &key, T &value, const T &defaultValue) {
    if (mMode == TO_JSON)
        mJsonMap[key] = ToJson(value);
    else {
        std::map<std::string, Any>::const_iterator it = mJsonMap.find(key);
        if (it == mJsonMap.end())
            value = defaultValue;
        else
            FromJson(value, it->second);
    }
}

inline void Jsonizable::JsonWrapper::JsonizeAsString(const std::string &key, std::string &value) {
    if (mFastMagicNum == FAST_MODE_MAGIC_NUMBER) {
        return FastJsonizable::JsonWrapper::JsonizeAsString(key, value);
    } else {
        util::legacy::Any anyVal;
        AnyJsonize(key, anyVal);
        value = ToJsonString(anyVal);
    }
}

inline void
Jsonizable::JsonWrapper::JsonizeAsString(const std::string &key, std::string &value, const std::string &defaultValue) {
    if (mFastMagicNum == FAST_MODE_MAGIC_NUMBER) {
        return FastJsonizable::JsonWrapper::JsonizeAsString(key, value, defaultValue);
    } else {
        util::legacy::Any anyVal;
        AnyJsonize(key, anyVal, anyVal);
        if (!anyVal.IsEmpty()) {
            value = ToJsonString(anyVal);
        } else {
            value = defaultValue;
        }
    }
}

inline Any ToJson(const Jsonizable &t) {
    Jsonizable::JsonWrapper w;
    const_cast<Jsonizable &>(t).Jsonize(w);
    return w.GetMap();
}

inline void FromJson(Jsonizable &t, const Any &a) {
    Jsonizable::JsonWrapper w(a);
    t.Jsonize(w);
}

inline void FromRapidValue(Jsonizable &t, RapidValue &value) {
    if (!value.IsObject()) {
        UTIL_LEGACY_THROW(TypeNotMatchException, "type not match, expect Object but get: " + FastToJsonString(value));
    }
    Jsonizable::JsonWrapper w(&value);
    t.Jsonize(w);
}

inline void serializeToWriter(RapidWriter *writer, const Jsonizable &t) {
    Jsonizable::JsonWrapper w(writer);
    writer->StartObject();
    const_cast<Jsonizable &>(t).Jsonize(w);
    writer->EndObject();
}

} // namespace legacy
} // namespace util
