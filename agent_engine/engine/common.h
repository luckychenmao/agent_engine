#pragma once

namespace engine {

constexpr char kRpcServerThreadNum[] = "rpc_server_thread_num";
constexpr char kRpcServerIoThreadNum[] = "rpc_server_io_thread_num";
constexpr char kRpcServerQueueSize[] = "rpc_server_queue_size";

template <int N>
class DependMatcher {};

#define PP_CONCAT(a, b) a##b
#define INIT_ONCE_FUNC() INIT_ONCE_FUNC_IMPL
#define INIT_ONCE_FUNC_IMPL                                                                                            \
protected:                                                                                                             \
    void DefineDependModule() override {                                                                               \
        engine::DependMatcher<0> *matcher = nullptr;                                                                   \
        DependModuleHelperFunc(matcher);                                                                               \
    }                                                                                                                  \
    template <int T>                                                                                                   \
    void DependModuleHelperFunc(engine::DependMatcher<T> *matcher) {}

#define DEPEND_ON_IMPL(Module, member, index)                                                                          \
private:                                                                                                               \
    std::shared_ptr<Module> member;                                                                                    \
                                                                                                                       \
protected:                                                                                                             \
    void DependModuleHelperFunc(engine::DependMatcher<index> *matcher) {                                               \
        engine::DependMatcher<index + 1> *next = nullptr;                                                              \
        bind_funcs_[typeid(Module).name()] = [this](std::shared_ptr<engine::BaseModule> base_module) {                 \
            this->depend_modules_.emplace(std::make_pair(typeid(Module).name(), base_module));                         \
            this->member = std::dynamic_pointer_cast<Module>(base_module);                                             \
        };                                                                                                             \
        DependModuleHelperFunc(next);                                                                                  \
    }

#define DEPEND_ON_0(Module, member) INIT_ONCE_FUNC() DEPEND_ON_IMPL(Module, member, 0);
#define DEPEND_ON_1(Module, member) DEPEND_ON_IMPL(Module, member, 1);
#define DEPEND_ON_2(Module, member) DEPEND_ON_IMPL(Module, member, 2);
#define DEPEND_ON_3(Module, member) DEPEND_ON_IMPL(Module, member, 3);
#define DEPEND_ON_4(Module, member) DEPEND_ON_IMPL(Module, member, 4);
#define DEPEND_ON_5(Module, member) DEPEND_ON_IMPL(Module, member, 5);
#define DEPEND_ON_6(Module, member) DEPEND_ON_IMPL(Module, member, 6);

} // namespace engine
