#include "locked_queue.h"

template<typename T, typename Compare>
LockedPriorityQueue<T, Compare>::LockedPriorityQueue() {}

template<typename T, typename Compare>
void LockedPriorityQueue<T, Compare>::push(const T& value)
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    heap_.push(value);
}

template<typename T, typename Compare>
void LockedPriorityQueue<T, Compare>::pop()
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    heap_.pop();
}

template<typename T, typename Compare>
T LockedPriorityQueue<T, Compare>::top() const
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return heap_.top();
}

template<typename T, typename Compare>
bool LockedPriorityQueue<T, Compare>::empty() const
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    return heap_.empty();
}
