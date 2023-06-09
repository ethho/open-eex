#pragma once
#include "locked_queue.hpp"
#include "order.hpp"

class MatchingEngine {
public:
    MatchingEngine();
    void addOrder(const Bid& bid);
    void addOrder(const Ask& ask);
    void cancelOrder(Order& order);
    // void printOrderBook() const;
    // std::string printBuyOrders() const;
    // std::string printSellOrders() const;
private:
    LockedPriorityQueue<Bid, std::greater<Bid>> buyOrders_;
    LockedPriorityQueue<Ask, std::less<Ask>> sellOrders_;
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
