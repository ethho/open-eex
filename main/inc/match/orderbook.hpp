#pragma once
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <string>


template<typename T>
class LockedPriorityQueue {
public:
    LockedPriorityQueue();
    void push(const T& value);
    void pop();
    T top() const;
    bool empty() const;
    std::string toString() const;
private:
    std::priority_queue<T> heap_;
    mutable std::shared_mutex mutex_;
};
