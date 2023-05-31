#pragma once
#include <queue>
#include <mutex>

template<typename T>
class LockedPriorityQueue {
public:
    LockedPriorityQueue() {}

    void push(const T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        heap_.push(value);
    }

    void pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        heap_.pop();
    }

    T top() const {
        std::shared_lock<std::mutex> lock(mutex_);
        return heap_.top();
    }

    bool empty() const {
        std::shared_lock<std::mutex> lock(mutex_);
        return heap_.empty();
    }

private:
    std::priority_queue<T> heap_;
    mutable std::mutex mutex_;
};