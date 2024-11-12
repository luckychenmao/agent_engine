#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>

#include "engine/common/base_module.h"
#include "util/log.h"

namespace engine {
namespace common {
class ModuleManager {
    using ModuleMap = std::unordered_map<std::string, std::shared_ptr<BaseModule>>;
    using ModuleCreatorFuncMap = std::unordered_map<std::string, ModuleCreatorFunc>;

public:
    ModuleManager();
    ~ModuleManager();
    ModuleManager(const ModuleManager &) = delete;
    ModuleManager &operator=(const ModuleManager &) = delete;

public:
    bool Init();
    bool Start();
    bool Stop();

public:
    template <typename ModuleType>
    std::shared_ptr<ModuleType> GetModule();

private:
    bool CreateModule();
    bool BindDependModule();
    bool Load(const std::shared_ptr<BaseModule> &module, int32_t depth = 0);
    bool Unload(const std::shared_ptr<BaseModule> &module, int32_t depth = 0);
    std::shared_ptr<BaseModule> GetModule(const std::string &name);

private:
    bool stopped_ = true;
    std::mutex mutex_;
    ModuleMap modules_;
    ModuleCreatorFuncMap module_creator_funcs_;

private:
    static constexpr int32_t MaxDepth = 64;

private:
    LOG_DECLARE();
};

template <typename ModuleType>
std::shared_ptr<ModuleType> ModuleManager::GetModule() {
    std::string module_name(typeid(ModuleType).name());
    auto iter = modules_.find(module_name);
    if (iter == modules_.end()) {
        LOG(WARN, "module not found, module_name[%s]", module_name.c_str());
        return nullptr;
    }
    return std::dynamic_pointer_cast<ModuleType>(iter->second);
}
} // namespace common
} // namespace engine
