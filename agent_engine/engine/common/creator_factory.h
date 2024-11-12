#pragma once

#include "engine/common/base_module.h"
#include "util/singleton.h"

namespace engine {
namespace common {
enum class RegistryType {
    kCreatorTypeModule
};

template <RegistryType Type>
struct RegistryTypeTraits;

template <RegistryType Type>
struct RegistryTypeTraits {
    typedef void CreatorFunc;
};

#define CREATOR_TYPE_HELPER(Type, Func)                                                                                \
    template <>                                                                                                        \
    struct RegistryTypeTraits<Type> {                                                                                  \
        typedef Func CreatorFunc;                                                                                      \
    }

CREATOR_TYPE_HELPER(RegistryType::kCreatorTypeModule, ModuleCreatorFunc);

template <RegistryType Type>
class CreatorFactory {
    using CreatorFuncType = typename RegistryTypeTraits<Type>::CreatorFunc;
    using CreatorFuncMap = std::unordered_map<std::string, CreatorFuncType>;

public:
    void AddCreatorFunc(const std::string &name, CreatorFuncType type) { creator_func_map_[name] = std::move(type); }

    CreatorFuncType GetCreatorFunc(const std::string &name = "default") {
        auto iter = creator_func_map_.find(name);
        if (iter == creator_func_map_.end()) {
            return CreatorFuncType();
        }
        return iter->second;
    }

    const CreatorFuncMap &GetCreatorFuncMap() { return creator_func_map_; }

private:
    CreatorFuncMap creator_func_map_;
};

#define REGISTER_MODULE(Module)                                                                                        \
    __attribute__((constructor)) static void Register##Module() {                                                      \
        util::Singleton<engine::common::CreatorFactory<RegistryType::kCreatorTypeModule>>::GetInstance()               \
            ->AddCreatorFunc(typeid(Module).name(), []() -> std::shared_ptr<engine::common::BaseModule> {              \
                auto module = std::make_shared<Module>();                                                              \
                module->SetName(typeid(Module).name());                                                                \
                return module;                                                                                         \
            });                                                                                                        \
    }
} // namespace common
} // namespace engine
