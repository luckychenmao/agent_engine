#include "engine/common/base_module.h"

namespace engine {
namespace common {
LOG_SETUP(engine.common, BaseModule);

BaseModule::BaseModule() {}

BaseModule::~BaseModule() { Clear(); }

bool BaseModule::Init(ModuleManager *manager) {
    if (!manager) {
        LOG(ERROR, "Module manager is null");
        return false;
    }
    manager_ = manager;
    if (!DoInit()) {
        LOG(ERROR, "Failed to initialize module %s", name_.c_str());
        return false;
    }
    return true;
}

bool BaseModule::Load() {
    if (loaded_) {
        LOG(DEBUG, "Module %s already loaded", name_.c_str());
        return true;
    }
    if (!DoLoad()) {
        LOG(ERROR, "Failed to load module %s", name_.c_str());
        return false;
    }
    loaded_ = true;
    return true;
}

bool BaseModule::Unload() {
    if (!loaded_) {
        LOG(DEBUG, "Module %s already unloaded", name_.c_str());
        return true;
    }
    if (!DoUnload()) {
        LOG(ERROR, "Failed to unload module %s", name_.c_str());
        return false;
    }
    loaded_ = false;
    return true;
}

void BaseModule::BeDepended(const std::string &name, const std::shared_ptr<BaseModule> &module) {
    be_depended_modules_[name] = module;
}

void BaseModule::Clear() {
    depend_modules_.clear();
    be_depended_modules_.clear();
    bind_funcs_.clear();
}

} // namespace common
} // namespace engine
