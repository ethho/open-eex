#include "gtest/gtest.h"
#include <orderbook.hpp>

TEST(LockedPriorityQueueTest, PushAndTop) {
    LockedPriorityQueue<int> queue;
    queue.push(3);
    queue.push(1);
    queue.push(4);
    queue.push(1);
    EXPECT_EQ(queue.top(), 1);
}

// TEST(LockedPriorityQueueTest, Pop) {
//     LockedPriorityQueue<int> queue;
//     queue.push(3);
//     queue.push(1);
//     queue.push(4);
//     queue.push(1);
//     queue.pop();
//     EXPECT_EQ(queue.top(), 4);
// }

// TEST(LockedPriorityQueueTest, Empty) {
//     LockedPriorityQueue<int> queue;
//     EXPECT_TRUE(queue.empty());
//     queue.push(3);
//     EXPECT_FALSE(queue.empty());
//     queue.pop();
//     EXPECT_TRUE(queue.empty());
// }
