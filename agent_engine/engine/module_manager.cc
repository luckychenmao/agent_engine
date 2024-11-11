#include "engine/module_manager.h"

#include "engine/creator_factory.h"

using namespace util;
namespace engine {
LOG_SETUP(engine, ModuleManager);

ModuleManager::ModuleManager() {}

ModuleManager::~ModuleManager() { Stop(); }

bool ModuleManager::Init() {
    module_creator_funcs_ =
        Singleton<CreatorFactory<engine::RegistryType::kCreatorTypeModule>>::GetInstance()->GetCreatorFuncMap();
    std::lock_guard _(mutex_);
    if (!CreateModule()) {
        LOG(ERROR, "create module failed");
        return false;
    }
    if (!BindDependModule()) {
        LOG(ERROR, "bind depend module failed");
        return false;
    }
    return true;
}

bool ModuleManager::Start() {
    std::lock_guard _(mutex_);
    if (!stopped_) {
        return true;
    }
    LOG(INFO, "module manager has started");
    for (auto &[name, module] : modules_) {
        if (!Load(module)) {
            LOG(ERROR, "module start failed, module[%s]", name.c_str());
            return false;
        }
    }
    stopped_ = false;
    return true;
}

bool ModuleManager::Stop() {
    std::lock_guard _(mutex_);
    if (stopped_) {
        return true;
    }
    LOG(INFO, "module manager has stopped");
    for (auto &[name, module] : modules_) {
        if (!Unload(module)) {
            LOG(ERROR, "module stop failed, module[%s]", name.c_str());
            return false;
        }
    }
    stopped_ = true;
    return true;
}

bool ModuleManager::CreateModule() {
    for (auto &[name, func] : module_creator_funcs_) {
        if (!func) {
            LOG(ERROR, "module regist failed, module type[%s]", name.c_str());
            return false;
        }
        auto module = func();
        if (!module) {
            LOG(ERROR, "module create failed, module type[%s]", name.c_str());
            return false;
        }
        if (!module->Init(this)) {
            LOG(ERROR, "module init failed, module name[%s]", name.c_str());
            return false;
        }
        modules_.emplace(name, std::move(module));
    }
    return true;
}

bool ModuleManager::BindDependModule() {
    for (auto &[module_name, module] : modules_) {
        module->InitDepends();
        auto bind_funcs = module->GetBindFuncs();
        for (auto &[depend_module_name, func] : bind_funcs) {
            auto depend_module = GetModule(depend_module_name);
            if (!depend_module) {
                LOG(ERROR,
                    "module bind failed, module[%s] depend module[%s] not exist",
                    module_name.c_str(),
                    depend_module_name.c_str());
                return false;
            }
            func(depend_module);
            depend_module->BeDepended(module_name, module);
        }
    }
    return true;
}

bool ModuleManager::Load(const std::shared_ptr<BaseModule> &module, int32_t depth) {
    if (depth >= MaxDepth) {
        LOG(ERROR, "module load failed, depth[%d] > MaxDepth[%d]", depth, MaxDepth);
        return false;
    }
    for (auto &[depend_module_name, depend_module] : module->GetDependModules()) {
        if (!depend_module) {
            LOG(ERROR,
                "module load failed, module[%s] depend module[%s] not exist",
                module->GetName().c_str(),
                depend_module_name.c_str());
            return false;
        }
        if (depend_module->IsLoaded()) {
            continue;
        }
        if (!Load(depend_module, depth + 1)) {
            LOG(ERROR,
                "module load failed, module[%s] depend module[%s] load failed",
                module->GetName().c_str(),
                depend_module_name.c_str());
            return false;
        }
    }
    if (!module->Load()) {
        LOG(ERROR, "module load failed, module[%s]", module->GetName().c_str());
        return false;
    }
    LOG(INFO, "module load success, module[%s]", module->GetName().c_str());
    return true;
}

bool ModuleManager::Unload(const std::shared_ptr<BaseModule> &module, int32_t depth) {
    if (depth >= MaxDepth) {
        LOG(ERROR, "module unload failed, depth[%d] > MaxDepth[%d]", depth, MaxDepth);
        return false;
    }
    for (auto &[depended_module_name, depended_module] : module->GetBeDependedModules()) {
        if (!depended_module) {
            LOG(ERROR,
                "module unload failed, module[%s] be depended module[%s] not exist",
                module->GetName().c_str(),
                depended_module_name.c_str());
            return false;
        }
        if (!depended_module->IsLoaded()) {
            continue;
        }
        if (!Unload(depended_module, depth + 1)) {
            LOG(ERROR,
                "module unload failed, module[%s] depend module[%s] unload failed",
                module->GetName().c_str(),
                depended_module_name.c_str());
            return false;
        }
    }
    if (!module->Unload()) {
        LOG(ERROR, "module unload failed, module[%s]", module->GetName().c_str());
        return false;
    }
    LOG(INFO, "module unload success, module[%s]", module->GetName().c_str());
    return true;
}

std::shared_ptr<BaseModule> ModuleManager::GetModule(const std::string &module_name) {
    auto it = modules_.find(module_name);
    if (it == modules_.end()) {
        LOG(WARN, "module not exist, module[%s]", module_name.c_str());
        return nullptr;
    }
    return it->second;
}
} // namespace engine
