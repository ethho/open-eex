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
protected:
    std::string symbol_;
    double price_;
    double volume_;
    TimePoint timePlaced_;
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
    Bid b0 {"AAPL", 10.0, 100.0};
    Bid b1 {"AAPL", 10.0, 100.0};
    Bid b2 {"AAPL", 50.0, 100.0};
    Bid b3 {"AAPL", 40.0, 10.0};
    Ask a0 {"AAPL", 50.0, -100.0};
    Ask a1 {"AAPL", 50.0, -100.0};
    Ask a2 {"AAPL", 60.0, -100.0};

    // Check operator overloads, which represent price-time priority
    CHECK(b0 != b1); // Different timePlaced
    CHECK(b0 > b1); // Same price, but b0 placed earlier
    CHECK(b1 < b2);
    CHECK(b2 > b1);
    CHECK(b2 > b3);

    // Check overloads for asks
    CHECK(a0 > a2);
    CHECK(a2 > a0);
    CHECK(a0 != a1);
    CHECK(a0 > a1); // Same price, but a0 placed earlier
}
#endif
