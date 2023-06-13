#pragma once
#include <iostream>
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <memory>

template<typename T, typename Compare = std::less<T>>
class LockedPriorityQueue {
public:
    LockedPriorityQueue();
    void push(const T& value);
    void pop();
    T top() const;
    bool empty() const;
    void print() const;
private:
    mutable std::shared_mutex mutex_;
    std::priority_queue<T, std::vector<T>, Compare> heap_;
};

//we need this code here so that when the compiler runs into an object of the templated class with given T, it knows how to create the source code for that object. The linker does not do this
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

template<typename T, typename Compare>
void LockedPriorityQueue<T, Compare>::print() const
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    std::priority_queue<T, std::vector<T>, Compare> heapCopy = heap_;
    while (!heapCopy.empty()) {
        std::cout << heapCopy.top() << "\n" << std::endl;
        heapCopy.pop();
    }
}

#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest/doctest.h"
TEST_CASE("test the locked queue int template")
{
    LockedPriorityQueue<int> q;
	q.push(1);
    CHECK(q.top() == 1);
	q.push(3);
	q.push(6);
    CHECK(q.top() == 6);
}

TEST_CASE("test the locked queue double template")
{
    LockedPriorityQueue<double> q;
	q.push(1.1);
    CHECK(q.top() == 1.1);
	q.push(3.3);
	q.push(6.7);
    CHECK(q.top() == 6.7);
}

TEST_CASE("test a min heap")
{
    LockedPriorityQueue<double, std::greater<double>> q;
	q.push(1.1);
    CHECK(q.top() == 1.1);
	q.push(3.3);
	q.push(6.7);
    CHECK(q.top() == 1.1);
}
#endif
