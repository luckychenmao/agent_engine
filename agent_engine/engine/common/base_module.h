#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "engine/common/common.h"
#include "util/log.h"

namespace engine {
namespace common {
class BaseModule;
class ModuleManager;

using ModuleMap = std::unordered_map<std::string, std::shared_ptr<BaseModule>>;
using ModuleCreatorFunc = std::function<std::shared_ptr<BaseModule>()>;
using ModuleBindFunc = std::function<void(std::shared_ptr<BaseModule>)>;
using BindFuncMap = std::unordered_map<std::string, ModuleBindFunc>;

class BaseModule {
public:
    BaseModule();
    virtual ~BaseModule();
    BaseModule(const BaseModule &) = delete;
    BaseModule &operator=(const BaseModule &) = delete;

public:
    bool Init(ModuleManager *manager);
    bool Load();
    bool Unload();
    void SetName(const std::string &name) { name_ = name; }
    const std::string &GetName() const { return name_; }
    bool IsLoaded() const { return loaded_; }

public:
    void InitDepends() { DefineDependModule(); }
    BindFuncMap &GetBindFuncs() { return bind_funcs_; }
    ModuleMap &GetDependModules() { return depend_modules_; }
    ModuleMap &GetBeDependedModules() { return be_depended_modules_; }
    void BeDepended(const std::string &name, const std::shared_ptr<BaseModule> &module);

protected:
    virtual bool DoInit() { return true; }
    virtual bool DoLoad() = 0;
    virtual bool DoUnload() = 0;
    virtual void DefineDependModule() {}

private:
    void Clear();

private:
    std::string name_;
    std::atomic<bool> loaded_ = false;
    ModuleManager *manager_ = nullptr;
    ModuleMap depend_modules_;
    ModuleMap be_depended_modules_;
    BindFuncMap bind_funcs_;

private:
    LOG_DECLARE();
};

} // namespace common
} // namespace engine
