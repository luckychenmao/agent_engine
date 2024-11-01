#include "util/EnvUtilImpl.h"

namespace util {

LOG_SETUP(util, EnvUtilImpl);

std::string EnvUtilImpl::GetEnv(const std::string &key, const std::string &defaultValue) {
    ScopedReadLock lock(_rwLock);
    const char *str = std::getenv(key.c_str());
    return str == nullptr ? defaultValue : std::string(str);
}

bool EnvUtilImpl::SetEnv(const std::string &env, const std::string &value, bool overwrite) {
    LOG(INFO, "setenv [%s], value [%s]", env.c_str(), value.c_str());
    ScopedWriteLock lock(_rwLock);
    return setenv(env.c_str(), value.c_str(), overwrite ? 1 : 0) == 0;
}

bool EnvUtilImpl::UnsetEnv(const std::string &env) {
    LOG(INFO, "unset env [%s]", env.c_str());
    ScopedWriteLock lock(_rwLock);
    return unsetenv(env.c_str()) == 0;
}

bool EnvUtilImpl::HasEnv(const std::string &env) {
    ScopedReadLock lock(_rwLock);
    const char *str = std::getenv(env.c_str());
    return str != nullptr;
}

std::string EnvUtilImpl::EnvReplace(const std::string &value) {
    size_t first_dollar = value.find('$');
    if (first_dollar == std::string::npos) {
        return value;
    }
    std::string ret = value.substr(0, first_dollar);
    const char *ptr = value.c_str() + first_dollar;
    const char *end = value.c_str() + value.size();
    const char *var_begin = NULL;
    ScopedReadLock lock(_rwLock);
    for (; ptr < end; ++ptr) {
        if (*ptr == '$' && *(ptr + 1) == '{') {
            if (var_begin != NULL) {
                ret.append(var_begin, ptr - var_begin);
            }
            var_begin = ptr;
            continue;
        } else if (*ptr == '}' && var_begin != NULL) {
            std::string key(var_begin + 2, ptr - var_begin - 2);
            const char *str = std::getenv(key.c_str());
            ret.append(str == NULL ? "" : std::string(str));
            var_begin = NULL;
        } else if (var_begin == NULL) {
            ret.push_back(*ptr);
        }
    }
    if (var_begin != NULL) {
        ret.append(var_begin);
    }
    return ret;
}
} // namespace util
