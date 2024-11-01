#pragma once

#include <cmath>
#include <cstdlib>
#include <cxxabi.h>
#include <functional>
#include <iomanip>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace util {

struct ToStringTagUniversal {};
struct ToStringTagStd {};
struct ToStringTagComplex {};

template <typename T, bool = std::is_arithmetic<T>::value>
struct ToStringTagTraits {
    typedef ToStringTagUniversal tag;
};
template <typename T>
struct ToStringTagTraits<T, true> {
    typedef ToStringTagStd tag;
};
template <>
struct ToStringTagTraits<float> {
    typedef ToStringTagUniversal tag;
};
template <>
struct ToStringTagTraits<double> {
    typedef ToStringTagComplex tag;
};
template <>
struct ToStringTagTraits<uint8_t> {
    typedef ToStringTagUniversal tag;
};
template <>
struct ToStringTagTraits<int8_t> {
    typedef ToStringTagUniversal tag;
};

class StringUtil {
public:
    static const std::string kNullString;
    typedef bool (*CMP_CHAR_FUNC)(const char a, const char b);

public:
    StringUtil();
    ~StringUtil();

public:
    static void Trim(std::string &str);
    static void Trim(std::string_view &str);
    static bool StartsWith(const std::string &str, const std::string &prefix);
    static bool EndWith(const std::string &str, const std::string &suffix);
    static std::vector<std::string> Split(const std::string &text, const std::string &sepStr, bool ignoreEmpty = true);
    static std::vector<std::string> Split(const std::string &text, const char &sepChar, bool ignoreEmpty = true);
    static void
    Split(std::vector<std::string> &vec, const std::string &text, const char &sepChar, bool ignoreEmpty = true);
    static void
    Split(std::vector<std::string> &vec, const std::string &text, const std::string &sepStr, bool ignoreEmpty = true);
    static bool IsSpace(const std::string &text);

    static void ToUpperCase(char *str);
    static void ToUpperCase(const char *str, std::string &retStr);
    static void ToUpperCase(std::string &str);
    static void ToLowerCase(char *str);
    static void ToLowerCase(std::string &str);
    static void ToLowerCase(const char *str, std::string &retStr);

    static bool StrToInt8(const char *str, int8_t &value);
    static bool StrToUInt8(const char *str, uint8_t &value);
    static bool StrToInt16(const char *str, int16_t &value);
    static bool StrToUInt16(const char *str, uint16_t &value);
    static bool StrToInt32(const char *str, int32_t &value);
    static bool StrToUInt32(const char *str, uint32_t &value);
    static bool StrToInt64(const char *str, int64_t &value);
    static bool StrToUInt64(const char *str, uint64_t &value);
    static bool StrToFloat(const char *str, float &value);
    static bool StrToDouble(const char *str, double &value);
    static bool HexStrToUint64(const char *str, uint64_t &value);
    static bool HexStrToInt64(const char *str, int64_t &value);
    static void Uint64ToHexStr(uint64_t value, char *hexStr, int len);

    static std::string StrToHexStr(const std::string &str);
    static std::string HexStrToStr(const std::string &hexStr);

    static uint32_t DeSerializeUInt32(const std::string &str);
    static void SerializeUInt32(uint32_t value, std::string &str);

    static uint64_t DeSerializeUInt64(const std::string &str);
    static void SerializeUInt64(uint64_t value, std::string &str);

    static int8_t StrToInt8WithDefault(const char *str, int8_t defaultValue);
    static uint8_t StrToUInt8WithDefault(const char *str, uint8_t defaultValue);
    static int16_t StrToInt16WithDefault(const char *str, int16_t defaultValue);
    static uint16_t StrToUInt16WithDefault(const char *str, uint16_t defaultValue);
    static int32_t StrToInt32WithDefault(const char *str, int32_t defaultValue);
    static uint32_t StrToUInt32WithDefault(const char *str, uint32_t defaultValue);
    static int64_t StrToInt64WithDefault(const char *str, int64_t defaultValue);
    static uint64_t StrToUInt64WithDefault(const char *str, uint64_t defaultValue);
    static float StrToFloatWithDefault(const char *str, float defaultValue);
    static double StrToDoubleWithDefault(const char *str, double defaultValue);

    static void Replace(std::string &str, char oldValue, char newValue);
    static void ReplaceFirst(std::string &str, const std::string &oldStr, const std::string &newStr);
    static void ReplaceLast(std::string &str, const std::string &oldStr, const std::string &newStr);
    static void ReplaceAll(std::string &str, const std::string &oldStr, const std::string &newStr);

    static const std::string &GetValueFromMap(const std::string &key, const std::map<std::string, std::string> &map);

    template <typename T, typename... U>
    static std::string ToString(const T &value, U &&...args);
    template <typename T, typename... U>
    static std::string ToString(const T *value, const size_t count, U &&...args);
    template <typename T, typename... U>
    static std::string ToString(const std::vector<T> &value, U &&...args);
    template <typename T, typename... U>
    static std::string ToStringImpl(ToStringTagComplex, const T &value, U &&...args);
    template <typename T, typename... U>
    static std::string ToStringImpl(ToStringTagUniversal, const T &value, U &&...args);
    template <typename T, typename... U>
    static std::string ToStringImpl(ToStringTagUniversal, const T *value, const size_t count, U &&...args);
    template <typename T>
    static std::string ToStringImpl(ToStringTagStd, const T &value);

    template <typename T>
    static void ToStream(std::ostream &os, const T &x);

    template <typename T>
    static void ToStream(std::ostream &os, const std::shared_ptr<T> &x);

    template <typename T>
    static void ToStream(std::ostream &os, const std::unique_ptr<T> &x);

    template <typename T>
    static T FromString(const std::string &str);

    template <typename T>
    static bool FromString(const std::string &str, T &value);

    template <typename T>
    static void FromString(const std::vector<std::string> &strVec, std::vector<T> &vec);

    template <typename T>
    static void FromString(const std::string &str, std::vector<T> &vec, const std::string &delim);

    template <typename T>
    static void FromString(const std::string &str,
                           std::vector<std::vector<T>> &vec,
                           const std::string &delim,
                           const std::string &delim2);

    template <typename T>
    static void ToStream(std::ostream &os, const std::vector<T> &x, const std::string &delim = " ");

    template <typename T>
    static void ToStream(std::ostream &os, const std::vector<std::string> &x, const std::string &delim = " ");

    template <typename T>
    static void ToStream(std::ostream &os, const std::set<T> &x, const std::string &delim = " ");

    template <typename T>
    static void ToStream(std::ostream &os, const std::unordered_set<T> &x, const std::string &delim = " ");

    template <typename T, typename E>
    static void ToStream(std::ostream &os,
                         const std::map<T, E> &x,
                         const std::string &first = ":",
                         const std::string &second = ";");

    template <typename T, typename E>
    static void ToStream(std::ostream &os,
                         const std::unordered_map<T, E> &x,
                         const std::string &first = ":",
                         const std::string &second = ";");
    template <typename T, typename E>
    static void ToStream(std::ostream &os, const std::pair<T, E> &x, const std::string &delim = ",");

    template <typename Iterator>
    static void ToStream(std::ostream &os, Iterator first, Iterator last, const std::string &delim = " ");

    template <typename T>
    static void ToStream(std::ostream &os,
                         const std::vector<std::vector<T>> &x,
                         const std::string &delim1,
                         const std::string &delim2);

    static void ToStream(std::ostream &os, const double &x, int32_t precision = 15);
    static void ToStream(std::ostream &os, const float &x, int32_t precision = 6);

    template <typename T>
    static void ToStream(
        std::ostream &os,
        const T *data,
        size_t count,
        const std::string &delim = " ",
        std::function<void(std::ostream &, const T &)> printer = [](std::ostream &os, const T &value) {
            return StringUtil::ToStream<T>(os, value);
        });

    static bool ParseTrueFalse(const std::string &str, bool &ret);

    template <typename T>
    static std::string ToBinaryString(T x);

    template <typename T>
    static bool FromBinaryString(const std::string &str, T &x);

    template <typename... Args>
    static std::string formatString(const std::string &format, Args... args);

    static std::string Join(const std::vector<std::string> &array, const std::string &seperator);

private:
    static std::stringstream *GetStringStream();
    static void PutStringStream(std::stringstream *ss);
    friend class StringStreamWrapper;
    class StringStreamWrapper {
    public:
        StringStreamWrapper(const std::string &str = "") : ss_(StringUtil::GetStringStream()) { ss_->str(str); }
        ~StringStreamWrapper() { StringUtil::PutStringStream(ss_); }
        template <typename T>
        StringStreamWrapper &operator<<(const T &x) {
            *ss_ << x;
            return *this;
        }
        template <typename T>
        StringStreamWrapper &operator>>(T &x) {
            *ss_ >> x;
            return *this;
        }
        std::string str() { return ss_->str(); }
        bool eof() { return ss_->eof(); }
        std::ostream &get() { return *ss_; }

    private:
        std::stringstream *ss_;
    };
};

template <typename T, typename... U>
inline std::string StringUtil::ToString(const T &value, U &&...args) {
    typename ToStringTagTraits<T>::tag tag;
    return ToStringImpl(tag, value, std::forward<U>(args)...);
}

template <typename T, typename... U>
inline std::string StringUtil::ToString(const T *value, const size_t count, U &&...args) {
    StringStreamWrapper oss;
    ToStream(oss.get(), value, count, std::forward<U>(args)...);
    return oss.str();
}

template <typename T, typename... U>
inline std::string StringUtil::ToString(const std::vector<T> &value, U &&...args) {
    return ToStringImpl(ToStringTagUniversal(), value, std::forward<U>(args)...);
}

template <typename T, typename... U>
inline std::string StringUtil::ToStringImpl(ToStringTagComplex, const T &value, U &&...args) {
    static_assert(std::is_floating_point<T>::value);
    if (std::fabs(value) >= std::numeric_limits<T>::max() * 0.5) {
        return ToStringImpl(ToStringTagStd(), value);
    }
    return ToStringImpl(ToStringTagUniversal(), value, std::forward<U>(args)...);
}

template <typename T, typename... U>
inline std::string StringUtil::ToStringImpl(ToStringTagUniversal, const T &value, U &&...args) {
    StringStreamWrapper oss;
    ToStream(oss.get(), value, std::forward<U>(args)...);
    return oss.str();
}

template <typename T, typename... U>
inline std::string StringUtil::ToStringImpl(ToStringTagUniversal, const T *value, const size_t count, U &&...args) {
    StringStreamWrapper oss;
    ToStream(oss.get(), value, count, std::forward<U>(args)...);
    return oss.str();
}

template <typename T>
inline std::string StringUtil::ToStringImpl(ToStringTagStd, const T &value) {
    return std::to_string(value);
}

template <typename T>
inline T StringUtil::FromString(const std::string &str) {
    T value = T();
    FromString(str, value);
    return value;
}

template <typename T>
inline bool StringUtil::FromString(const std::string &str, T &value) {
    StringStreamWrapper iss(str);
    iss >> value;
    return iss.eof();
}

template <>
inline bool StringUtil::FromString(const std::string &str, std::string &value) {
    value = str;
    return true;
}

template <>
inline bool StringUtil::FromString(const std::string &str, int8_t &value) {
    bool ret = StrToInt8(str.c_str(), value);
    return ret;
}

template <>
inline bool StringUtil::FromString(const std::string &str, uint8_t &value) {
    bool ret = StrToUInt8(str.c_str(), value);
    return ret;
}

template <>
inline bool StringUtil::FromString(const std::string &str, int16_t &value) {
    bool ret = StrToInt16(str.data(), value);
    return ret;
}

template <>
inline bool StringUtil::FromString(const std::string &str, uint16_t &value) {
    bool ret = StrToUInt16(str.c_str(), value);
    return ret;
}

template <>
inline bool StringUtil::FromString(const std::string &str, int32_t &value) {
    bool ret = StrToInt32(str.c_str(), value);
    return ret;
}

template <>
inline bool StringUtil::FromString(const std::string &str, uint32_t &value) {
    bool ret = StrToUInt32(str.c_str(), value);
    return ret;
}

template <>
inline bool StringUtil::FromString(const std::string &str, int64_t &value) {
    bool ret = StrToInt64(str.c_str(), value);
    return ret;
}

template <>
inline bool StringUtil::FromString(const std::string &str, uint64_t &value) {
    bool ret = StrToUInt64(str.c_str(), value);
    return ret;
}

template <>
inline bool StringUtil::FromString(const std::string &str, float &value) {
    bool ret = StrToFloat(str.c_str(), value);
    return ret;
}

template <>
inline bool StringUtil::FromString(const std::string &str, double &value) {
    bool ret = StrToDouble(str.c_str(), value);
    return ret;
}

template <>
inline bool StringUtil::FromString(const std::string &str, bool &value) {
    return ParseTrueFalse(str, value);
}

template <typename T>
inline void StringUtil::ToStream(std::ostream &os, const T &x) {
    os << x;
}

template <>
inline void StringUtil::ToStream<int8_t>(std::ostream &os, const int8_t &x) {
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", x);
    os << buf;
}

template <>
inline void StringUtil::ToStream<uint8_t>(std::ostream &os, const uint8_t &x) {
    char buf[8];
    snprintf(buf, sizeof(buf), "%u", x);
    os << buf;
}

template <typename T>
inline void StringUtil::ToStream(std::ostream &os, const std::shared_ptr<T> &x) {
    os << x.get();
}

template <typename T>
inline void StringUtil::ToStream(std::ostream &os, const std::unique_ptr<T> &x) {
    os << x.get();
}

inline void StringUtil::ToStream(std::ostream &os, const double &x, int32_t precision) {
    auto ss = os.precision();
    os << std::setprecision(precision) << x << std::setprecision(ss);
}

inline void StringUtil::ToStream(std::ostream &os, const float &x, int32_t precision) {
    auto ss = os.precision();
    os << std::setprecision(precision) << x << std::setprecision(ss);
}

template <typename T>
inline void StringUtil::ToStream(std::ostream &os, const std::vector<T> &x, const std::string &delim) {
    ToStream(os, x.begin(), x.end(), delim);
}

template <typename T>
inline void StringUtil::ToStream(std::ostream &os, const std::vector<std::string> &x, const std::string &delim) {
    for (auto it = x.begin(); it != x.end(); ++it) {
        if (it != x.begin())
            os << delim;
        ToStream(os, *it);
    }
}

template <typename Iterator>
inline void StringUtil::ToStream(std::ostream &os, Iterator first, Iterator last, const std::string &delim) {
    for (Iterator it = first; it != last; ++it) {
        if (it != first)
            os << delim;
        ToStream(os, *it);
    }
}

template <typename T>
inline void StringUtil::ToStream(std::ostream &os,
                                 const std::vector<std::vector<T>> &x,
                                 const std::string &delim1,
                                 const std::string &delim2) {
    auto first = x.begin();
    for (typename std::vector<std::vector<T>>::const_iterator it = first; it != x.end(); ++it) {
        if (it != first)
            os << delim2;
        ToStream(os, *it, delim1);
    }
}

template <typename T>
inline void StringUtil::ToStream(std::ostream &os, const std::set<T> &x, const std::string &delim) {
    ToStream(os, x.begin(), x.end(), delim);
}

template <typename T>
inline void StringUtil::ToStream(std::ostream &os, const std::unordered_set<T> &x, const std::string &delim) {
    ToStream(os, x.begin(), x.end(), delim);
}

template <typename T, typename E>
inline void
StringUtil::ToStream(std::ostream &os, const std::map<T, E> &x, const std::string &first, const std::string &second) {
    for (auto iter = x.begin(); iter != x.end(); ++iter) {
        ToStream(os, iter->first);
        os << first;
        ToStream(os, iter->second);
        os << second;
    }
}

template <typename T, typename E>
inline void StringUtil::ToStream(std::ostream &os,
                                 const std::unordered_map<T, E> &x,
                                 const std::string &first,
                                 const std::string &second) {
    for (auto iter = x.begin(); iter != x.end(); ++iter) {
        ToStream(os, iter->first);
        os << first;
        ToStream(os, iter->second);
        os << second;
    }
}

template <typename T, typename E>
inline void StringUtil::ToStream(std::ostream &os, const std::pair<T, E> &x, const std::string &delim) {
    os << "(";
    ToStream(os, x.first);
    os << delim;
    ToStream(os, x.second);
    os << ")";
}

template <typename T>
inline void StringUtil::ToStream(std::ostream &os,
                                 const T *data,
                                 size_t count,
                                 const std::string &delim,
                                 std::function<void(std::ostream &, const T &)> printer) {
    for (size_t i = 0; i < count; i++) {
        if (i != 0) {
            os << delim;
        }
        printer(os, data[i]);
    }
}

template <typename T>
inline void StringUtil::FromString(const std::vector<std::string> &strVec, std::vector<T> &vec) {
    vec.clear();
    vec.reserve(strVec.size());
    for (uint32_t i = 0; i < strVec.size(); ++i) {
        vec.push_back(FromString<T>(strVec[i]));
    }
}

template <typename T>
inline void StringUtil::FromString(const std::string &str, std::vector<T> &vec, const std::string &delim) {
    std::vector<std::string> strVec = Split(str, delim);
    FromString(strVec, vec);
}

template <typename T>
inline void StringUtil::FromString(const std::string &str,
                                   std::vector<std::vector<T>> &vec,
                                   const std::string &delim1,
                                   const std::string &delim2) {
    vec.clear();
    std::vector<std::string> strVec;
    FromString(str, strVec, delim2);
    vec.resize(strVec.size());
    for (uint32_t i = 0; i < strVec.size(); ++i) {
        FromString(strVec[i], vec[i], delim1);
    }
}

template <typename T>
inline std::string StringUtil::ToBinaryString(T x) {
    std::string str;
    str.resize(sizeof(T));
    *((T *)str.data()) = x;
    return str;
}

template <typename T>
bool StringUtil::FromBinaryString(const std::string &str, T &x) {
    if (str.length() != sizeof(T)) {
        return false;
    }
    x = *((T *)str.data());
    return true;
}

inline const std::string &StringUtil::GetValueFromMap(const std::string &key,
                                                      const std::map<std::string, std::string> &map) {
    std::map<std::string, std::string>::const_iterator it = map.find(key);
    if (it != map.end()) {
        return it->second;
    }
    return kNullString;
}

template <typename... Args>
std::string StringUtil::formatString(const std::string &format, Args... args) {
    int size_buf = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
    if (size_buf <= 0) {
        return kNullString;
    }
    std::unique_ptr<char[]> buf(new (std::nothrow) char[size_buf]);
    if (!buf) {
        return kNullString;
    }
    int len = std::snprintf(buf.get(), size_buf, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + len);
}

} // namespace util
