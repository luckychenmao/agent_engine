#include "util/string_tokenizer.h"

#include "util/log.h"

using namespace std;
namespace util {
DECLARE_AND_SETUP_LOGGER(util, StringTokenizer);

StringTokenizer::StringTokenizer(const string &str, const string &sep, Option opt) { Tokenize(str, sep, opt); }

StringTokenizer::StringTokenizer() {}

StringTokenizer::~StringTokenizer() {}

size_t StringTokenizer::Tokenize(const string &str, const string &sep, Option opt) {
    Tokenize(str, sep, sep.length(), tokens_, opt);
    return tokens_.size();
}

} // namespace util
