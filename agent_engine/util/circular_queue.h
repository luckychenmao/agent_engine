#pragma once

#include <assert.h>
#include <cstddef>
#include <stdint.h>
#include <utility>

namespace util {

template <typename T>
class CircularQueue {
public:
    CircularQueue(size_t capacity = 4 * 1024) {
        assert(capacity < ((1L << 31) - 1));
        items_ = new T[capacity];
        capacity_ = capacity;
        size_ = 0;
        front_ = 0;
        back_ = 0;
    }
    ~CircularQueue() { delete[] items_; }

private:
    CircularQueue(const CircularQueue &);
    CircularQueue &operator=(const CircularQueue &);

public:
    template <typename Value>
    inline void push_front(Value &&t) {
        assert(size_ < capacity_);
        if (size_ == 0) {
            front_ = 0;
            back_ = 0;
        } else {
            if (--front_ == -1) {
                front_ = capacity_ - 1;
            }
        }
        ++size_;
        items_[front_] = std::forward<Value>(t);
    }

    template <typename Value>
    inline void push_back(Value &&t) {
        assert(size_ < capacity_);
        if (size_ == 0) {
            front_ = 0;
            back_ = 0;
        } else {
            if (++back_ == capacity_) {
                back_ = 0;
            }
        }
        ++size_;
        items_[back_] = std::forward<Value>(t);
    }

    inline void pop_front() {
        assert(size_);
        if (++front_ == capacity_) {
            front_ = 0;
        }
        --size_;
    }

    inline void pop_back() {
        assert(size_);
        if (--back_ == -1) {
            back_ = capacity_ - 1;
        }
        --size_;
    }

    inline T &front() { return items_[front_]; }
    inline T &back() { return items_[back_]; }

    inline const T &front() const { return items_[front_]; }
    inline const T &back() const { return items_[back_]; }

    inline size_t size() const { return size_; }
    inline size_t capacity() const { return capacity_; }
    inline bool empty() const { return size_ == 0; }

private:
    T *items_;
    int32_t capacity_;
    int32_t size_;
    int32_t front_;
    int32_t back_;
};

} // namespace util
