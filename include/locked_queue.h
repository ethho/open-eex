#pragma once
#include <queue>
#include <mutex>
#include <shared_mutex>


template<typename T>
class LockedPriorityQueue {
public:
    LockedPriorityQueue<T>();
    /**
     * Push a new value to the queue.
     */
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
TEST_CASE("test the locked queue")
{
    LockedPriorityQueue<int> q {};
	q.push(1);
    CHECK(q.top() == 1);
	q.push(3);
	q.push(6);
    CHECK(q.top() == 6);
}
#endif
