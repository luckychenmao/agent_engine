#include "util/thread.h"

#include <cstddef>
#include <functional>
#include <memory>
#include <pthread.h>
#include <string>

#include "util/log.h"

namespace util {
DECLARE_AND_SETUP_LOGGER(util, Thread);
struct ThreadFunctionWrapperArg {
    std::function<void()> threadFunction;
    std::string name;
};

ThreadNameScope::ThreadNameScope(const std::string &name) : name_(name), self_(pthread_self()) {
    if (name_.size() >= MaxLength) {
        name_.resize(MaxLength);
        name_[MaxLength - 1] = '\0';
    }
    ok_ = (0 == pthread_getname_np(self_, current_name_, MaxLength));
    if (ok_) {
        pthread_setname_np(self_, name_.c_str());
    }
}

ThreadNameScope::~ThreadNameScope() {
    if (ok_) {
        pthread_setname_np(self_, current_name_);
    }
}

void *Thread::ThreadWrapperFunction(void *arg) {
    std::unique_ptr<ThreadFunctionWrapperArg> p(static_cast<ThreadFunctionWrapperArg *>(arg));
    LOG(INFO, "thread started for name[%s]", p->name.c_str());
    p->threadFunction();
    return nullptr;
}

ThreadPtr Thread::CreateThread(const std::function<void()> &threadFunction, const std::string &name) {
    ThreadPtr threadPtr(new Thread);
    ThreadFunctionWrapperArg *arg = new ThreadFunctionWrapperArg;
    arg->threadFunction = threadFunction;
    arg->name = name;

    std::unique_ptr<ThreadNameScope> scope;
    if (!name.empty()) {
        scope.reset(new ThreadNameScope(name));
    }
    int rtn = pthread_create(&threadPtr->id_, nullptr, &Thread::ThreadWrapperFunction, arg);

    if (rtn != 0) {
        delete arg;
        threadPtr->id_ = 0;
        threadPtr.reset();
        LOG(ERROR, "Create thread error [%d]", rtn);
    }
    return threadPtr;
}

} // namespace util
