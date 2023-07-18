#pragma once
#include "locked_queue.hpp"
#include "order.hpp"

class Server;

class MatchingEngine {
public:
    MatchingEngine();
    void addOrder(const Order& order);
    void addOrder(const Bid& bid);
    void addOrder(const Ask& ask);
    void cancelOrder(Order& order);
    // void printOrderBook() const;
    // std::string printBuyOrders() const;
    // std::string printSellOrders() const;
    std::string printOrders() const;
    Bid top_buy();
    Ask top_sell();
    bool any_buy_orders();
    bool any_sell_orders();
    bool any_orders();
    void adjust_heaps();
    void pop_buy();
    void pop_sell();
private:
    LockedPriorityQueue<Bid, std::greater<Bid>> buyOrders_;
    LockedPriorityQueue<Ask, std::less<Ask>> sellOrders_;
    std::unordered_map<int, Order> orders_;
    std::vector<Order> all_orders_;
    // std::unordered_map<std::string, int> orderIds_;
    // std::unordered_map<std::string, int> orderCounts_;
    // std::unordered_map<std::string, double> orderPrices_;
    // std::unordered_map<std::string, double> orderVolumes_;
    friend std::ostream& operator<<(std::ostream& os, const MatchingEngine& me);
    double eps;
};

std::ostream& operator<<(std::ostream& os, const MatchingEngine& me);


#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest/doctest.h"
TEST_CASE("test matching engine initialization")
{
    MatchingEngine me;
    std::stringstream ss;
    ss << me;
    //This unit test has changed
    CHECK(ss.str().find("All orders:") != std::string::npos);
}
#endif
