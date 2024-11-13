#pragma once

namespace util {

class WorkItem {
public:
    WorkItem() {}
    virtual ~WorkItem() {}
    virtual void process() = 0;

public:
    virtual void destroy() { delete this; }
    virtual void drop() { destroy(); }
};

class LambdaWorkItem : public WorkItem {
public:
    LambdaWorkItem(std::function<void()> func) : func_(std::move(func)) {}

public:
    void process() override {
        if (func_) {
            func_();
        }
    }

private:
    std::function<void()> func_;
};

} // namespace util
