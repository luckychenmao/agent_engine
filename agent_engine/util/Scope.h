#pragma once

#include <functional>
#include <utility>

namespace util {

class ScopeGuard {
public:
    template <typename F>
    ScopeGuard(F &&f) : exit_func_(std::forward<F>(f)) {}

    ~ScopeGuard() {
        if (exit_func_) {
            exit_func_();
        }
    }
    void Release() { exit_func_ = std::function<void()>(); }

private:
    std::function<void()> exit_func_;
};

} // namespace util
