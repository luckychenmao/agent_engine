#pragma once

#include <functional>
#include <utility>

namespace util {

class ScopeGuard {
public:
    template <typename F>
    ScopeGuard(F &&f) : _exitFunc(std::forward<F>(f)) {}

    ~ScopeGuard() {
        if (_exitFunc) {
            _exitFunc();
        }
    }
    void release() { _exitFunc = std::function<void()>(); }

private:
    std::function<void()> _exitFunc;
};

} // namespace util
