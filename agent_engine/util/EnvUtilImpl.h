#pragma once

#include <mutex>
#include <stdlib.h>
#include <string>

#include "util/Log.h"
#include "util/Singleton.h"
#include "util/StringUtil.h"

namespace util {

class EnvUtilImpl {
public:
    template <typename T>
    T GetEnv(const std::string &key, const T &defaultValue) {
        std::lock_guard<std::mutex> _(lock_);
        const char *str = std::getenv(key.c_str());
        if (!str) {
            return defaultValue;
        }
        T ret = T();
        auto success = StringUtil::FromString(str, ret);
        if (success) {
            return ret;
        } else {
            LOG(WARN,
                "failed to parse env [%s:%s] to typed, use default value [%s]",
                key.c_str(),
                str,
                StringUtil::ToString(defaultValue).c_str());
            return defaultValue;
        }
    }
    template <typename T>
    bool GetEnvWithoutDefault(const std::string &key, T &value) {
        std::lock_guard<std::mutex> _(lock_);
        const char *str = std::getenv(key.c_str());
        if (!str) {
            return false;
        }
        return StringUtil::FromString(str, value);
    }

    std::string GetEnv(const std::string &key, const std::string &defaulValue = "");

    bool SetEnv(const std::string &env, const std::string &value, bool overwrite = true);

    bool UnsetEnv(const std::string &env);

    bool HasEnv(const std::string &env);

    std::string EnvReplace(const std::string &value);

private:
    std::mutex lock_;

private:
    LOG_DECLARE();
};
} // namespace util
