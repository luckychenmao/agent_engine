#pragma once
#include <mutex>

namespace util {

template <typename T>
class Singleton {
public:
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

protected:
    Singleton() {}
    ~Singleton() {}

public:
    static T *GetInstance() {
        static T instance;
        return &instance;
    }
};

} // namespace util
