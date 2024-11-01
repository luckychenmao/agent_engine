#pragma once

#include <algorithm>
#include <assert.h>
#include <cctype>
#include <cstddef>
#include <stdint.h>
#include <string>
#include <string_view>
#include <vector>

#include "util/CommonMacros.h"

namespace util {

class StringTokenizer {
public:
    /// ignore empty tokens
    const static int32_t kTokenIgnoreEmpty = 1;
    /// remove leading and trailing whitespace from tokens
    const static int32_t kTokenTrim = 2;
    /// leave the token as it is
    const static int32_t kTokenLeaveAs = 4;

    typedef int32_t Option;
    typedef std::vector<std::string> TokenVector;
    typedef TokenVector::const_iterator Iterator;

public:
    StringTokenizer(const std::string &str, const std::string &sep, Option opt = kTokenLeaveAs);
    StringTokenizer();
    ~StringTokenizer();

public:
    size_t Tokenize(const std::string &str, const std::string &sep, Option opt = kTokenLeaveAs);

    inline Iterator begin() const;
    inline Iterator end() const;

    inline const std::string &operator[](size_t index) const;

    inline size_t GetNumTokens() const;
    inline const TokenVector &GetTokenVector() const;

public:
    template <typename RawStringType>
    static std::vector<std::string_view>
    TokenizeView(const RawStringType &str, const std::string &sep, Option opt = kTokenIgnoreEmpty | kTokenTrim);

    template <typename RawStringType>
    static std::vector<std::string_view>
    TokenizeView(const RawStringType &str, char c, Option opt = kTokenIgnoreEmpty | kTokenTrim);

private:
    template <typename Sep, typename ResultContainerType, typename RawStringType>
    inline static void
    Tokenize(const RawStringType &str, Sep &&sep, size_t sepLen, ResultContainerType &results, Option opt)
        __attribute__((always_inline));
    inline static bool IsSpace(char ch);

    template <typename ResultContainerType, typename ResultItemType>
    inline static void AddToken(const ResultItemType &token, Option opt, ResultContainerType &target)
        __attribute__((always_inline));

private:
    StringTokenizer(const StringTokenizer &);
    StringTokenizer &operator=(const StringTokenizer &);

private:
    TokenVector tokens_;
};

///////////////////////////////////////////////////////////
// inlines

template <typename ResultContainerType, typename ResultItemType>
void StringTokenizer::AddToken(const ResultItemType &token, Option opt, ResultContainerType &target) {
    size_t length = token.size();
    const char *data = token.data();
    if (unlikely(opt & kTokenLeaveAs)) {
        if (!(opt & kTokenIgnoreEmpty)) {
            target.push_back(ResultItemType(data, length));
        } else if (length > 0) {
            target.push_back(ResultItemType(data, length));
        }
    } else if (unlikely(opt & kTokenTrim)) {
        size_t n = 0;
        while (n < length && IsSpace(data[n])) {
            n++;
        }
        size_t n2 = length;
        while (n2 > n && IsSpace(data[n2 - 1])) {
            n2--;
        }
        if (n2 > n) {
            target.push_back(ResultItemType(data + n, n2 - n));
        } else if (!(opt & kTokenIgnoreEmpty)) {
            target.push_back(ResultItemType()); // insert an empty token
        }
    } else if (length > 0) {
        target.push_back(ResultItemType(data, length));
    }
}

template <typename Sep, typename ResultContainerType, typename RawStringType>
inline void StringTokenizer::Tokenize(
    const RawStringType &str, Sep &&sep, size_t sepLen, ResultContainerType &results, Option opt) {
    typedef typename ResultContainerType::value_type T;
    size_t n = 0, old = 0;
    while (n != std::string::npos) {
        n = str.find(sep, n);
        if (n != std::string::npos) {
            if (n != old) {
                T subStr(str.data() + old, n - old);
                AddToken(subStr, opt, results);
            } else {
                T subStr("");
                AddToken(subStr, opt, results);
            }

            n += sepLen;
            old = n;
        }
    }
    T subStr(str.data() + old, str.size() - old);
    AddToken(subStr, opt, results);
}

inline bool StringTokenizer::IsSpace(char ch) { return (ch > 0 && std::isspace(ch)); }

inline StringTokenizer::Iterator StringTokenizer::begin() const { return tokens_.begin(); }

inline StringTokenizer::Iterator StringTokenizer::end() const { return tokens_.end(); }

inline const std::string &StringTokenizer::operator[](std::size_t index) const {
    assert(index < tokens_.size());
    return tokens_[index];
}

inline size_t StringTokenizer::GetNumTokens() const { return tokens_.size(); }

inline const StringTokenizer::TokenVector &StringTokenizer::GetTokenVector() const { return tokens_; }

template <typename RawStringType>
std::vector<std::string_view>
StringTokenizer::TokenizeView(const RawStringType &str, const std::string &sep, Option opt) {
    std::vector<std::string_view> ret;
    Tokenize(str, sep, sep.length(), ret, opt);
    return ret;
}
template <typename RawStringType>
std::vector<std::string_view> StringTokenizer::TokenizeView(const RawStringType &str, char c, Option opt) {
    std::vector<std::string_view> ret;
    Tokenize(str, c, 1, ret, opt);
    return ret;
}
} // namespace util
