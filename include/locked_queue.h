#pragma once
#include <queue>
#include <mutex>
#include <shared_mutex>


template<typename T>
class LockedPriorityQueue {
public:
    LockedPriorityQueue<T>();
    void push(const T& value);
    void pop();
    T top() const;
    bool empty() const;
private:
    mutable std::shared_mutex mutex_;
    std::priority_queue<T> heap_;
};

// Subclass of LockedPriorityQueue that allows for a custom comparator
template<typename T, typename Compare>
class LockedPriorityQueueWithComparator : public LockedPriorityQueue<T> {
public:
    LockedPriorityQueueWithComparator<T, Compare>() : LockedPriorityQueue<T>() {}
    void push(const T& value) {
        std::unique_lock<std::shared_mutex> lock(this->mutex_);
        this->heap_.push(value, comparator_);
    }
private:
    Compare comparator_;
};

// Min heap comparator
template<typename T>
struct MinHeapComparator {
    bool operator()(const T& lhs, const T& rhs) const {
        return lhs > rhs;
    }
};

// Max heap comparator
template<typename T>
struct MaxHeapComparator {
    bool operator()(const T& lhs, const T& rhs) const {
        return lhs < rhs;
    }
};

// Subclass of LockedPriorityQueue that uses the MinHeapComparator
template<typename T>
class MinHeap : public LockedPriorityQueueWithComparator<T, MinHeapComparator<T>> {
public:
    MinHeap<T>() : LockedPriorityQueueWithComparator<T, MinHeapComparator<T>>() {}
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
//     MinHeap<double> q {};
// 	q.push(1.1);
//     CHECK(q.top() == 1.1);
// 	q.push(3.3);
// 	q.push(6.7);
//     CHECK(q.top() == 1.1);
// }
#endif
