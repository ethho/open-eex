#pragma once
#include <locked_queue.h>
#include <order.h>

class MatchingEngine {
public:
    MatchingEngine();
    void addOrder(const Order& order);
    void cancelOrder(const Order& order);
    void printOrderBook() const;
    std::string printBuyOrders() const;
    std::string printSellOrders() const;
protected:
    LockedPriorityQueue<Order, std::greater<Order>> buyOrders_;
    LockedPriorityQueue<Order, std::greater<Order>> sellOrders_;
    std::unordered_map<int, Order> orders_;
    // std::unordered_map<std::string, int> orderIds_;
    // std::unordered_map<std::string, int> orderCounts_;
    // std::unordered_map<std::string, double> orderPrices_;
    // std::unordered_map<std::string, double> orderVolumes_;
};

ostream& operator<<(ostream& os, const MatchingEngine& me);


#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest/doctest.h"
TEST_CASE("test matching engine initialization")
{
    MatchingEngine me;
    std::stringstream ss;
    ss << me;
    CHECK(ss.str().find("Buy Orders:") != std::string::npos);
}
#endif
