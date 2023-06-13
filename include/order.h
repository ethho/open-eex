#pragma once
#include <string>
#include <sstream>
#include <chrono>

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

class Order {
public:
    Order();
    Order(const std::string& symbol, double price, double volume);
    virtual ~Order() = default;
    std::string symbol() const;
    double price() const;
    double volume() const;
    TimePoint timePlaced() const;
    bool isActive() const;
    void deactivate();
    bool operator==(const Order& rhs) const;
    bool operator!=(const Order& rhs) const;
private:
    std::string symbol_;
    TimePoint timePlaced_;
    double price_;
    double volume_;
    bool isActive_;
};

class Bid: public Order {
public:
    Bid();
    Bid(const std::string& symbol, double price, double volume);
    virtual ~Bid() = default;
    /*
     * Comparator overloads sort by price-time priority.
     * If order1 has a better price than order2, then order1 is given priority
     * and order1 > order2.
     */
    bool operator<(const Bid& rhs) const;
    bool operator>(const Bid& rhs) const;
};

class Ask: public Order {
public:
    Ask();
    Ask(const std::string& symbol, double price, double volume);
    /*
     * Comparator overloads sort by price-time priority.
     * If order1 has a better price than order2, then order1 is given priority
     * and order1 > order2.
     */
    bool operator<(const Ask& rhs) const;
    bool operator>(const Ask& rhs) const;
};

std::ostream& operator<<(std::ostream& os, Order const & order);

#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest/doctest.h"
TEST_CASE("test order initialization")
{
    Order o;
    CHECK(o.symbol() == "AAPL");
    CHECK(o.price() == 0.0);
    CHECK(o.volume() == 0.0);

    std::stringstream ss;
    ss << o;
    // CHECK(ss.str() == "foobar");
    CHECK(ss.str().find("Order") == 0);
}

TEST_CASE("test operator overloads")
{
    Order o1 {"AAPL", 10.0, 100.0};
    Order o2 {"AAPL", 50.0, 100.0};
    Order o3 {"AAPL", 40.0, -10.0};
    Order o4 {"AAPL", 50.0, 100.0};
    Order o5 {"AAPL", 50.0, 100.0};
    CHECK(o1 < o2);
    CHECK(o2 > o3);
    CHECK(o2 == o4);

    // Due to time priority
    CHECK(o4 != o5);
    CHECK(o5 < o4);
    CHECK(o4 > o5);
}
#endif
