
#pragma once

#include <string>

#include "util/legacy/exception.h"

namespace util {
namespace legacy {

class NotJsonizableException : public ExceptionBase {
public:
    UTIL_LEGACY_DEFINE_EXCEPTION(NotJsonizableException, ExceptionBase);
};

class TypeNotMatchException : public ExceptionBase {
public:
    UTIL_LEGACY_DEFINE_EXCEPTION(TypeNotMatchException, ExceptionBase);
};

class WrongFormatJsonException : public ExceptionBase {
public:
    UTIL_LEGACY_DEFINE_EXCEPTION(WrongFormatJsonException, ExceptionBase);
};

class NotSupportException : public ExceptionBase {
public:
    UTIL_LEGACY_DEFINE_EXCEPTION(NotSupportException, ExceptionBase);
};

} // namespace legacy
} // namespace util
