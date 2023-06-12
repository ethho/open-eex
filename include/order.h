#pragma once
#include <string>

class Order {
public:
    Order();
    Order(const std::string& symbol, double price, double volume);
    std::string symbol() const;
    double price() const;
    double volume() const;
    bool isSell() const;
    bool isBuy() const;
    bool isActive() const;
    void deactivate();
    bool operator<(const Order& rhs) const;
    bool operator>(const Order& rhs) const;
    bool operator==(const Order& rhs) const;
private:
    std::string symbol_;
    double price_;
    double volume_;
    bool isActive_;
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
}

TEST_CASE("test operator overloads")
{
    Order o1 {"AAPL", 10.0, 100.0};
    Order o2 {"AAPL", 50.0, 100.0};
    Order o3 {"AAPL", 40.0, -10.0};
    Order o4 {"AAPL", 50.0, 100.0};
    CHECK(o1 < o2);
    CHECK(o2 > o3);
    CHECK(o2 == o4);
}
#endif
