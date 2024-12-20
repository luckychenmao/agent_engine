
#pragma once

#if defined(__SSE4_2__)
#ifndef UTIL_RAPIDJSON_SSE42
#define UTIL_RAPIDJSON_SSE42
#endif
#elif defined(__SSE2__)
#ifndef UTIL_RAPIDJSON_SSE2
#define UTIL_RAPIDJSON_SSE2
#endif
#endif

#include <map>
#include <stdint.h>
#include <string>

#include "util/legacy/exception.h"
#include "util/legacy/fast_jsonizable.h"
#include "util/legacy/jsonizable_exception.h"
// #include "util/legacy/fast_jsonizable_any.h"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace util {
namespace legacy {

class FastJsonizableBase {
public:
    enum Mode {
        TO_JSON,
        FROM_JSON
    };

    class JsonWrapper {
    public:
        JsonWrapper() : mMode(TO_JSON) {}

        JsonWrapper(RapidWriter *writer) : mMode(TO_JSON), mWriter(writer) {}

        JsonWrapper(RapidValue *value) : mMode(FROM_JSON), mValue(value) {}

        Mode GetMode() const { return mMode; }

        RapidValue *GetRapidValue() const { return mValue; }

        template <typename T>
        void Jsonize(const std::string &key, const T &value) {
            assert(mMode == TO_JSON);
            FastJsonize(key, value);
        }

        template <typename T>
        void Jsonize(const std::string &key, T &value) {
            FastJsonize(key, value);
        }

        template <typename T>
        void Jsonize(const std::string &key, T &value, const T &defaultValue) {
            FastJsonize(key, value, defaultValue);
        }

        inline void JsonizeAsString(const std::string &key, std::string &value);
        inline void JsonizeAsString(const std::string &key, std::string &value, const std::string &defautValue);

        void Jsonize(const std::string &key, std::string &value, const std::string &defaultValue) {
            return Jsonize<std::string>(key, value, defaultValue);
        }

        void Jsonize(const std::string &key, int64_t &value, const int64_t &defaultValue) {
            return Jsonize<int64_t>(key, value, defaultValue);
        }

    protected:
        inline void FastJsonize(const std::string &key, RapidValue *&value);
        inline void FastJsonize(const std::string &key, RapidValue *&value, RapidValue *const &defaultValue);

        template <typename T>
        void FastJsonize(const std::string &key, const T &value);

        template <typename T>
        void FastJsonize(const std::string &key, T &value);

        template <typename T>
        void FastJsonize(const std::string &key, T &value, const T &defaultValue);

    public:
        RapidValue *getRapidValue() const { return mValue; }

    protected:
        Mode mMode;

    private:
        RapidWriter *mWriter = nullptr;
        RapidValue *mValue = nullptr;
    };
};

class FastJsonizable : public FastJsonizableBase {
public:
    virtual ~FastJsonizable() {}
    using FastJsonizableBase::JsonWrapper;
    virtual void Jsonize(JsonWrapper &json) = 0;
};

inline void FastJsonizableBase::JsonWrapper::FastJsonize(const std::string &key, RapidValue *&value) {
    if (mMode == TO_JSON) {
        if (value) {
            mWriter->Key(key.c_str(), key.size());
            value->Accept(*mWriter);
        }
    } else {
        if (mValue->HasMember(key.c_str())) {
            value = &(*mValue)[key];
        } else {
            UTIL_LEGACY_THROW(NotJsonizableException, key + " not found when try to parse from Json.");
        }
    }
}

inline void FastJsonizableBase::JsonWrapper::FastJsonize(const std::string &key,
                                                         RapidValue *&value,
                                                         RapidValue *const &defaultValue) {
    if (mMode == TO_JSON) {
        if (value) {
            mWriter->Key(key.c_str(), key.size());
            value->Accept(*mWriter);
        }
    } else {
        if (mValue->HasMember(key.c_str())) {
            value = &(*mValue)[key];
        } else {
            value = defaultValue;
        }
    }
}

template <typename T>
void FastJsonizableBase::JsonWrapper::FastJsonize(const std::string &key, const T &value) {
    assert(mMode == TO_JSON);
    mWriter->Key(key.c_str(), key.size());
    serializeToWriter(mWriter, value);
}

template <typename T>
void FastJsonizableBase::JsonWrapper::FastJsonize(const std::string &key, T &value) {
    if (mMode == TO_JSON) {
        mWriter->Key(key.c_str(), key.size());
        serializeToWriter(mWriter, value);
    } else {
        if (mValue->HasMember(key.c_str())) {
            FromRapidValue(value, (*mValue)[key]);
        } else {
            UTIL_LEGACY_THROW(NotJsonizableException, key + " not found when try to parse from Json.");
        }
    }
}

template <typename T>
void FastJsonizableBase::JsonWrapper::FastJsonize(const std::string &key, T &value, const T &defaultValue) {
    if (mMode == TO_JSON) {
        mWriter->Key(key.c_str(), key.size());
        serializeToWriter(mWriter, value);
    } else {
        if (mValue->HasMember(key.c_str())) {
            FromRapidValue(value, (*mValue)[key]);
        } else {
            value = defaultValue;
        }
    }
}

inline void FastJsonizableBase::JsonWrapper::JsonizeAsString(const std::string &key, std::string &value) {
    RapidValue *jsonValue = nullptr;
    FastJsonize(key, jsonValue);
    if (jsonValue) {
        value = FastToJsonString(*jsonValue);
    }
}

inline void FastJsonizableBase::JsonWrapper::JsonizeAsString(const std::string &key,
                                                             std::string &value,
                                                             const std::string &defautValue) {
    RapidValue *jsonValue = nullptr;
    FastJsonize(key, jsonValue, jsonValue);
    if (jsonValue) {
        value = FastToJsonString(*jsonValue);
    } else {
        value = defautValue;
    }
}

inline void FromRapidValue(FastJsonizable &t, RapidValue &value) {
    if (!value.IsObject()) {
        UTIL_LEGACY_THROW(TypeNotMatchException, "type not match, expect Object but get: " + FastToJsonString(value));
    }
    FastJsonizable::JsonWrapper w(&value);
    t.Jsonize(w);
}

inline void serializeToWriter(RapidWriter *writer, const FastJsonizable &t) {
    FastJsonizable::JsonWrapper w(writer);
    writer->StartObject();
    const_cast<FastJsonizable &>(t).Jsonize(w);
    writer->EndObject();
}

} // namespace legacy
} // namespace util
