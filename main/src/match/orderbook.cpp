#include <orderbook.hpp>


template<typename T>
LockedPriorityQueue<T>::LockedPriorityQueue() {}

template<typename T>
void LockedPriorityQueue<T>::push(const T& value) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    heap_.push(value);
}

template<typename T>
void LockedPriorityQueue<T>::pop() {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    heap_.pop();
}

template<typename T>
T LockedPriorityQueue<T>::top() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return heap_.top();
}

template<typename T>
bool LockedPriorityQueue<T>::empty() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return heap_.empty();
}

template class LockedPriorityQueue<int>;
