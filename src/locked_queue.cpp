#include "locked_queue.h"

// Default constructor
template<typename T>
LockedPriorityQueue<T>::LockedPriorityQueue() {}

// Max heap comparator
template<typename T>
bool MaxHeapComparator<T>::operator()(const T& a, const T& b) const {
    return a > b;
}

// Min heap comparator
template<typename T>
bool MinHeapComparator<T>::operator()(const T& a, const T& b) const {
    return a < b;
}

// Constructor with custom comparator
template<typename T>
MinHeap<T>::MinHeap() {
    // Assign heap_ to a priority queue with a custom comparator
    heap_ = std::priority_queue<T, std::vector<T>, MinHeapComparator<T>> {};
}


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



#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest/doctest.h"
TEST_CASE("we can have tests written here, to test impl. details")
{
    CHECK(true);
}
#endif
