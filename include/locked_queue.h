#pragma once
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
private:
    mutable std::shared_mutex mutex_;
    std::priority_queue<T, std::vector<T>, Compare> heap_;
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

// TEST_CASE("test a min heap")
// {
//     LockedPriorityQueue<double, std::greater<double>> q;
// 	q.push(1.1);
//     CHECK(q.top() == 1.1);
// 	q.push(3.3);
// 	q.push(6.7);
//     CHECK(q.top() == 1.1);
// }
#endif
