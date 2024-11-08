#pragma once
#include <string>

#include "util/legacy/RapidJsonCommon.h"

namespace util {
namespace legacy {

class JsonTestUtil {
public:
    static void checkJsonStringEqual(const std::string &expected, const std::string &actual);

private:
    static bool deepSortKeys(SimpleValue &value);
    static void prettyDocument(SimpleDocument &document, std::string &result);
};

} // namespace legacy
} // namespace util
