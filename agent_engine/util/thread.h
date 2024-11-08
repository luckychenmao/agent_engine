#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <pthread.h>
#include <stddef.h>
#include <string>

namespace util {

class Thread;

using ThreadPtr = std::shared_ptr<Thread>;
using ThreadId = pthread_t;

class ThreadNameScope {
private:
    static constexpr int MaxLength = 16;

public:
    ThreadNameScope(const std::string &name);
    ~ThreadNameScope();

private:
    std::string name_;
    pthread_t self_;
    char current_name_[MaxLength];
    bool ok_;
};

class Thread {
public:
    ~Thread() { Join(); }
    static ThreadPtr CreateThread(const std::function<void()> &threadFunction, const std::string &name = "");
    void Join() {
        if (id_) {
            int ret = pthread_join(id_, nullptr);
            (void)ret;
            assert(ret == 0);
        }
        id_ = 0;
    }
    pthread_t GetId() const { return id_; }

private:
    Thread() { id_ = 0; }

private:
    static void *ThreadWrapperFunction(void *arg);

private:
    pthread_t id_;
};

} // namespace util
