#pragma once

#include <stdlib.h>
#include <string>

#include "util/EnvUtilImpl.h"
#include "util/Log.h"
#include "util/Singleton.h"

namespace util {

class EnvUtil {
public:
    template <typename T>
    static T GetEnv(const std::string &key, const T &defaultValue) {
        EnvUtilImpl *impl = Singleton<EnvUtilImpl>::getInstance();
        return impl->GetEnv(key, defaultValue);
    }
    template <typename T>
    static bool GetEnvWithoutDefault(const std::string &key, T &value) {
        EnvUtilImpl *impl = Singleton<EnvUtilImpl>::getInstance();
        return impl->GetEnvWithoutDefault(key, value);
    }

    static std::string GetEnv(const std::string &key, const std::string &defaulValue = "") {
        EnvUtilImpl *impl = Singleton<EnvUtilImpl>::getInstance();
        return impl->GetEnv(key, defaulValue);
    }

    static bool SetEnv(const std::string &env, const std::string &value, bool overwrite = true) {
        EnvUtilImpl *impl = Singleton<EnvUtilImpl>::getInstance();
        return impl->SetEnv(env, value, overwrite);
    }

    static bool UnsetEnv(const std::string &env) {
        EnvUtilImpl *impl = Singleton<EnvUtilImpl>::getInstance();
        return impl->UnsetEnv(env);
    }

    static bool HasEnv(const std::string &env) {
        EnvUtilImpl *impl = Singleton<EnvUtilImpl>::getInstance();
        return impl->HasEnv(env);
    }

    static std::string EnvReplace(const std::string &value) {
        EnvUtilImpl *impl = Singleton<EnvUtilImpl>::getInstance();
        return impl->EnvReplace(value);
    }
};

class EnvGuard {
public:
    EnvGuard(const std::string &key, const std::string &newValue) : key_(key) {
        key_exist_ = EnvUtil::GetEnvWithoutDefault(key, old_value_);
        EnvUtil::SetEnv(key, newValue);
    }
    ~EnvGuard() {
        if (!key_exist_) {
            EnvUtil::UnsetEnv(key_);
        } else {
            EnvUtil::SetEnv(key_, old_value_);
        }
    }

private:
    std::string key_;
    std::string old_value_;
    bool key_exist_ = false;
};

} // namespace util
