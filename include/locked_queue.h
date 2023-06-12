#pragma once
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <memory>

template<typename T>
class LockedPriorityQueue {
public:
    LockedPriorityQueue();
    void push(const T& value);
    void pop();
    T top() const;
    bool empty() const;
private:
    mutable std::shared_mutex mutex_;
    // std::priority_queue<T, std::vector<T>, std::less<T>> heap_;
};

template<typename T>
class MaxHeap: public LockedPriorityQueue<T> {
public:
    MaxHeap();
private:
    std::priority_queue<T, std::vector<T>, std::less<T>> heap_;
};


template<typename T>
class MinHeap: public LockedPriorityQueue<T> {
public:
    MinHeap();
private:
    std::priority_queue<T, std::vector<T>, std::greater<T>> heap_;
};

#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest/doctest.h"
TEST_CASE("test the locked queue int template")
{
    MaxHeap<int> q {};
	q.push(1);
    CHECK(q.top() == 1);
	q.push(3);
	q.push(6);
    CHECK(q.top() == 6);
}

TEST_CASE("test the locked queue double template")
{
    MaxHeap<double> q {};
	q.push(1.1);
    CHECK(q.top() == 1.1);
	q.push(3.3);
	q.push(6.7);
    CHECK(q.top() == 6.7);
}

TEST_CASE("test a min heap")
{
    MinHeap<double> q {};
	q.push(1.1);
    CHECK(q.top() == 1.1);
	q.push(3.3);
	q.push(6.7);
    CHECK(q.top() == 1.1);
}
#endif
