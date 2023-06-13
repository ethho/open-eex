#pragma once
#include "locked_queue.hpp"
#include "order.hpp"

class MatchingEngine {
public:
    MatchingEngine();
    void addOrder(const Order& order);
    void cancelOrder(Order& order);
    void printOrderBook() const;
private:
    LockedPriorityQueue<Order, std::greater<Order>> buyOrders_;
    LockedPriorityQueue<Order, std::less<Order>> sellOrders_;
    std::unordered_map<int, Order> orders_;
    // std::unordered_map<std::string, int> orderIds_;
    // std::unordered_map<std::string, int> orderCounts_;
    // std::unordered_map<std::string, double> orderPrices_;
    // std::unordered_map<std::string, double> orderVolumes_;
};



#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest/doctest.h"
TEST_CASE("test matching engine initialization")
{
    MatchingEngine me;
}
#endif
