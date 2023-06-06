#pragma once
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <functional>

// Base comparator class
template<typename T>
class Comparator {
public:
    virtual bool operator()(const T& a, const T& b) const = 0;
};

template<typename T>
class MaxHeapComparator : public Comparator<T> {
public:
    bool operator()(const T& a, const T& b) const override;
};

template<typename T>
class MinHeapComparator : public Comparator<T> {
public:
    bool operator()(const T& a, const T& b) const override;
};

template<typename T>
class LockedPriorityQueue {
public:
    LockedPriorityQueue();
    LockedPriorityQueue(Comparator<T> comparator);
    void push(const T& value);
    void pop();
    T top() const;
    bool empty() const;
private:
    mutable std::shared_mutex mutex_;
    std::priority_queue<T> heap_;
};


#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest/doctest.h"
TEST_CASE("test the locked queue int template")
{
    LockedPriorityQueue<int> q {};
	q.push(1);
    CHECK(q.top() == 1);
	q.push(3);
	q.push(6);
    CHECK(q.top() == 6);
}

TEST_CASE("test the locked queue double template")
{
    LockedPriorityQueue<double> q {};
	q.push(1.1);
    CHECK(q.top() == 1.1);
	q.push(3.3);
	q.push(6.7);
    CHECK(q.top() == 6.7);
}

TEST_CASE("test a min heap")
{
    LockedPriorityQueue<double> q;
	q.push(1.1);
    CHECK(q.top() == 1.1);
	q.push(3.3);
	q.push(6.7);
    CHECK(q.top() == 1.1);
}
#endif
