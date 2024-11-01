#include "util/string_util.h"

#include <algorithm>
#include <assert.h>
#include <bits/types/struct_tm.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

using namespace std;

namespace util {

const std::string StringUtil::kNullString = "";

StringUtil::StringUtil() {}

StringUtil::~StringUtil() {}

class StringStreamPool {
public:
    StringStreamPool() { _ssVec.reserve(8); }
    ~StringStreamPool() {
        for (size_t i = 0; i < _ssVec.size(); ++i) {
            delete _ssVec[i];
        }
    }
    stringstream *GetStringStream() {
        stringstream *ret = NULL;
        {
            std::lock_guard<std::mutex> _(lock_);
            if (!_ssVec.empty()) {
                ret = _ssVec.back();
                _ssVec.pop_back();
            }
        }
        if (ret == NULL) {
            ret = new stringstream();
        }
        return ret;
    }
    void PutStringStream(stringstream *ss) {
        ss->clear();
        std::lock_guard<std::mutex> _(lock_);
        _ssVec.push_back(ss);
    }

private:
    std::mutex lock_;
    vector<stringstream *> _ssVec;
};

static const size_t POOL_SIZE = 127;
static StringStreamPool sPool[POOL_SIZE];

stringstream *StringUtil::GetStringStream() {
    size_t offset = pthread_self() % POOL_SIZE;
    return sPool[offset].GetStringStream();
}

void StringUtil::PutStringStream(stringstream *ss) {
    size_t offset = pthread_self() % POOL_SIZE;
    sPool[offset].PutStringStream(ss);
}

void StringUtil::Trim(std::string &str) {
    str.erase(str.find_last_not_of(' ') + 1);
    str.erase(0, str.find_first_not_of(' '));
}

void StringUtil::Trim(std::string_view &str) {
    auto endPos = str.find_last_not_of(' ');
    auto beginPos = str.find_first_not_of(' ');
    if (beginPos == std::string_view::npos) {
        beginPos = 0;
    }
    str = str.substr(beginPos, endPos - beginPos + 1);
}

bool StringUtil::ParseTrueFalse(const string &str, bool &value) {
    string trimStr = str;
    Trim(trimStr);
    ToLowerCase(trimStr);
    if (trimStr == "t" || trimStr == "true" || trimStr == "y" || trimStr == "yes" || trimStr == "1") {
        value = true;
        return true;
    } else if (trimStr == "f" || trimStr == "false" || trimStr == "n" || trimStr == "no" || trimStr == "0") {
        value = false;
        return true;
    } else {
        return false;
    }
}

bool StringUtil::StartsWith(const std::string &str, const std::string &prefix) {
    return (str.size() >= prefix.size()) && (str.compare(0, prefix.size(), prefix) == 0);
}

bool StringUtil::EndWith(const std::string &str, const std::string &suffix) {
    size_t s1 = str.size();
    size_t s2 = suffix.size();
    return (s1 >= s2) && (str.compare(s1 - s2, s2, suffix) == 0);
}

std::vector<std::string> StringUtil::Split(const std::string &text, const std::string &sepStr, bool ignoreEmpty) {
    std::vector<std::string> vec;
    Split(vec, text, sepStr, ignoreEmpty);
    return vec;
}

std::vector<std::string> StringUtil::Split(const std::string &text, const char &sepChar, bool ignoreEmpty) {
    std::vector<std::string> vec;
    Split(vec, text, sepChar, ignoreEmpty);
    return vec;
}

void StringUtil::Split(std::vector<std::string> &vec, const std::string &text, const char &sepChar, bool ignoreEmpty) {
    Split(vec, text, std::string(1, sepChar), ignoreEmpty);
}

void StringUtil::Split(std::vector<std::string> &vec,
                       const std::string &text,
                       const std::string &sepStr,
                       bool ignoreEmpty) {
    std::string str(text);
    std::string sep(sepStr);
    size_t n = 0, old = 0;
    while (n != std::string::npos) {
        n = str.find(sep, n);
        if (n != std::string::npos) {
            if (!ignoreEmpty || n != old)
                vec.emplace_back(str.substr(old, n - old));
            n += sep.length();
            old = n;
        }
    }

    if (!ignoreEmpty || old < str.length()) {
        vec.emplace_back(str.substr(old, str.length() - old));
    }
}

bool StringUtil::IsSpace(const string &text) {
    if (text == string("　")) {
        return true;
    }
    if (text.length() > 1) {
        return false;
    }
    return isspace(text[0]);
}

void StringUtil::ToUpperCase(char *str) {
    if (str) {
        while (*str) {
            if (*str >= 'a' && *str <= 'z') {
                *str += 'A' - 'a';
            }
            str++;
        }
    }
}

void StringUtil::ToUpperCase(string &str) {
    for (size_t i = 0; i < str.size(); i++) {
        str[i] = toupper(str[i]);
    }
}

void StringUtil::ToUpperCase(const char *str, std::string &retStr) {
    retStr = str;
    for (size_t i = 0; i < retStr.size(); i++) {
        retStr[i] = toupper(str[i]);
    }
}

void StringUtil::ToLowerCase(string &str) {
    for (size_t i = 0; i < str.size(); i++) {
        str[i] = tolower(str[i]);
    }
}

void StringUtil::ToLowerCase(char *str) {
    if (str) {
        while (*str) {
            if (*str >= 'A' && *str <= 'Z') {
                *str -= 'A' - 'a';
            }
            str++;
        }
    }
}

void StringUtil::ToLowerCase(const char *str, std::string &retStr) {
    retStr = str;
    ToLowerCase(retStr);
}

bool StringUtil::StrToInt8(const char *str, int8_t &value) {
    int32_t v32 = 0;
    bool ret = StrToInt32(str, v32);
    value = (int8_t)v32;

    return ret && v32 >= INT8_MIN && v32 <= INT8_MAX;
}

bool StringUtil::StrToUInt8(const char *str, uint8_t &value) {
    uint32_t v32 = 0;
    bool ret = StrToUInt32(str, v32);
    value = (uint8_t)v32;

    return ret && v32 <= UINT8_MAX;
}

bool StringUtil::StrToInt16(const char *str, int16_t &value) {
    int32_t v32 = 0;
    bool ret = StrToInt32(str, v32);
    value = (int16_t)v32;
    return ret && v32 >= INT16_MIN && v32 <= INT16_MAX;
}

bool StringUtil::StrToUInt16(const char *str, uint16_t &value) {
    uint32_t v32 = 0;
    bool ret = StrToUInt32(str, v32);
    value = (uint16_t)v32;
    return ret && v32 <= UINT16_MAX;
}

bool StringUtil::StrToInt32(const char *str, int32_t &value) {
    if (NULL == str || *str == 0) {
        return false;
    }
    char *endPtr = NULL;
    errno = 0;

#if __WORDSIZE == 64
    int64_t value64 = strtol(str, &endPtr, 10);
    if (value64 < INT32_MIN || value64 > INT32_MAX) {
        return false;
    }
    value = (int32_t)value64;
#else
    value = (int32_t)strtol(str, &endPtr, 10);
#endif

    if (errno == 0 && endPtr && *endPtr == 0) {
        return true;
    }
    return false;
}

bool StringUtil::StrToUInt32(const char *str, uint32_t &value) {
    if (NULL == str || *str == 0 || *str == '-') {
        return false;
    }
    char *endPtr = NULL;
    errno = 0;

#if __WORDSIZE == 64
    uint64_t value64 = strtoul(str, &endPtr, 10);
    if (value64 > UINT32_MAX) {
        return false;
    }
    value = (int32_t)value64;
#else
    value = (uint32_t)strtoul(str, &endPtr, 10);
#endif

    if (errno == 0 && endPtr && *endPtr == 0) {
        return true;
    }
    return false;
}

bool StringUtil::StrToUInt64(const char *str, uint64_t &value) {
    if (NULL == str || *str == 0 || *str == '-') {
        return false;
    }
    char *endPtr = NULL;
    errno = 0;
    value = (uint64_t)strtoull(str, &endPtr, 10);
    if (errno == 0 && endPtr && *endPtr == 0) {
        return true;
    }
    return false;
}

bool StringUtil::StrToInt64(const char *str, int64_t &value) {
    if (NULL == str || *str == 0) {
        return false;
    }
    char *endPtr = NULL;
    errno = 0;
    value = (int64_t)strtoll(str, &endPtr, 10);
    if (errno == 0 && endPtr && *endPtr == 0) {
        return true;
    }
    return false;
}

bool StringUtil::HexStrToUint64(const char *str, uint64_t &value) {
    if (NULL == str || *str == 0) {
        return false;
    }
    char *endPtr = NULL;
    errno = 0;
    value = (uint64_t)strtoull(str, &endPtr, 16);
    if (errno == 0 && endPtr && *endPtr == 0) {
        return true;
    }
    return false;
}

bool StringUtil::HexStrToInt64(const char *str, int64_t &value) {
    if (NULL == str || *str == 0) {
        return false;
    }
    char *endPtr = NULL;
    errno = 0;
    value = (int64_t)strtoll(str, &endPtr, 16);
    if (errno == 0 && endPtr && *endPtr == 0) {
        return true;
    }
    return false;
}

uint32_t StringUtil::DeSerializeUInt32(const std::string &str) {
    assert(str.length() == sizeof(uint32_t));

    uint32_t value = 0;
    for (size_t i = 0; i < str.length(); ++i) {
        value <<= 8;
        value |= (unsigned char)str[i];
    }
    return value;
}

void StringUtil::SerializeUInt32(uint32_t value, std::string &str) {
    char key[4];
    for (int i = (int)sizeof(uint32_t) - 1; i >= 0; --i) {
        key[i] = (char)(value & 0xFF);
        value >>= 8;
    }
    str.assign(key, sizeof(uint32_t));
}

void StringUtil::SerializeUInt64(uint64_t value, std::string &str) {
    char key[8];
    for (int i = (int)sizeof(uint64_t) - 1; i >= 0; --i) {
        key[i] = (char)(value & 0xFF);
        value >>= 8;
    }
    str.assign(key, sizeof(uint64_t));
}

uint64_t StringUtil::DeSerializeUInt64(const std::string &str) {
    assert(str.length() == sizeof(uint64_t));

    uint64_t value = 0;
    for (size_t i = 0; i < str.length(); ++i) {
        value <<= 8;
        value |= (unsigned char)str[i];
    }
    return value;
}

bool StringUtil::StrToFloat(const char *str, float &value) {
    if (NULL == str || *str == 0) {
        return false;
    }
    errno = 0;
    char *endPtr = NULL;
    value = strtof(str, &endPtr);
    if (errno == 0 && endPtr && *endPtr == 0) {
        return true;
    }
    return false;
}

bool StringUtil::StrToDouble(const char *str, double &value) {
    if (NULL == str || *str == 0) {
        return false;
    }
    errno = 0;
    char *endPtr = NULL;
    value = strtod(str, &endPtr);
    if (errno == 0 && endPtr && *endPtr == 0) {
        return true;
    }
    return false;
}

void StringUtil::Uint64ToHexStr(uint64_t value, char *hexStr, int len) {
    assert(len > 16);
    snprintf(hexStr, len, "%016lx", value);
}

int8_t StringUtil::StrToInt8WithDefault(const char *str, int8_t defaultValue) {
    int8_t tmp;
    if (StrToInt8(str, tmp)) {
        return tmp;
    }
    return defaultValue;
}

uint8_t StringUtil::StrToUInt8WithDefault(const char *str, uint8_t defaultValue) {
    uint8_t tmp;
    if (StrToUInt8(str, tmp)) {
        return tmp;
    }
    return defaultValue;
}

int16_t StringUtil::StrToInt16WithDefault(const char *str, int16_t defaultValue) {
    int16_t tmp;
    if (StrToInt16(str, tmp)) {
        return tmp;
    }
    return defaultValue;
}

uint16_t StringUtil::StrToUInt16WithDefault(const char *str, uint16_t defaultValue) {
    uint16_t tmp;
    if (StrToUInt16(str, tmp)) {
        return tmp;
    }
    return defaultValue;
}

int32_t StringUtil::StrToInt32WithDefault(const char *str, int32_t defaultValue) {
    int32_t tmp;
    if (StrToInt32(str, tmp)) {
        return tmp;
    }
    return defaultValue;
}

uint32_t StringUtil::StrToUInt32WithDefault(const char *str, uint32_t defaultValue) {
    uint32_t tmp;
    if (StrToUInt32(str, tmp)) {
        return tmp;
    }
    return defaultValue;
}

int64_t StringUtil::StrToInt64WithDefault(const char *str, int64_t defaultValue) {
    int64_t tmp;
    if (StrToInt64(str, tmp)) {
        return tmp;
    }
    return defaultValue;
}

uint64_t StringUtil::StrToUInt64WithDefault(const char *str, uint64_t defaultValue) {
    uint64_t tmp;
    if (StrToUInt64(str, tmp)) {
        return tmp;
    }
    return defaultValue;
}

float StringUtil::StrToFloatWithDefault(const char *str, float defaultValue) {
    float tmp;
    if (StrToFloat(str, tmp)) {
        return tmp;
    }
    return defaultValue;
}

double StringUtil::StrToDoubleWithDefault(const char *str, double defaultValue) {
    double tmp;
    if (StrToDouble(str, tmp)) {
        return tmp;
    }
    return defaultValue;
}

void StringUtil::ReplaceLast(string &str, const string &oldStr, const string &newStr) {
    size_t pos = str.rfind(oldStr);
    if (pos != string::npos) {
        str.replace(pos, oldStr.size(), newStr);
    }
}

void StringUtil::ReplaceFirst(string &str, const string &oldStr, const string &newStr) {
    size_t pos = str.find(oldStr);
    if (pos != string::npos) {
        str.replace(pos, oldStr.size(), newStr);
    }
}

void StringUtil::ReplaceAll(string &str, const string &oldStr, const string &newStr) {
    size_t pos = str.find(oldStr);
    while (pos != std::string::npos) {
        str.replace(pos, oldStr.size(), newStr);
        pos = str.find(oldStr, pos + newStr.size());
    }
}

void StringUtil::Replace(string &str, char oldValue, char newValue) {
    ::replace(str.begin(), str.end(), oldValue, newValue);
}

string StringUtil::StrToHexStr(const std::string &str) {
    string hexStr;
    hexStr.resize(2 * str.length());

    for (size_t i = 0; i < str.length(); ++i) {
        uint8_t c = (uint8_t)str[i];
        sprintf(&hexStr[i * 2], "%02x", c);
    }

    return hexStr;
}

string StringUtil::HexStrToStr(const std::string &hexStr) {
    assert(hexStr.length() % 2 == 0);
    string str;
    string lowCasedStr;
    StringUtil::ToLowerCase(hexStr.c_str(), lowCasedStr);
    for (size_t i = 0; i < lowCasedStr.length(); i += 2) {
        int ascii;
        char a = lowCasedStr[i], b = lowCasedStr[i + 1];
        if (a >= 'a') {
            ascii = (a - 'a' + 10) * 16;
        } else {
            ascii = (a - '0') * 16;
        }

        if (b >= 'a') {
            ascii += (b - 'a' + 10);
        } else {
            ascii += (b - '0');
        }

        char tempChar = (char)ascii;
        str += tempChar;
    }
    return str;
}

string StringUtil::Join(const vector<string> &array, const string &seperator) {
    string result;
    for (vector<string>::const_iterator itr = array.begin(); itr != array.end(); ++itr) {
        result.append(*itr);
        if (itr + 1 != array.end()) {
            result.append(seperator);
        }
    }
    return result;
}

} // namespace util
