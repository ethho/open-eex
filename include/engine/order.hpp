#pragma once
#include <string>
#include <sstream>
#include <chrono>

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
#include <iostream>
#include <cmath>

class Client;

class Order {
public:
    Order();
    Order(const std::string& symbol, double price, double volume);
    virtual ~Order() = default;
    std::string symbol() const;
    int id() const;
    double price() const;
    double volume() const;
    TimePoint timePlaced() const;
    bool isActive() const;
    void deactivate();
    bool operator==(const Order& rhs) const;
    bool operator!=(const Order& rhs) const;
    friend std::ostream& operator<<(std::ostream& s, const Order& o);
    virtual std::string typeName() const;
    void update_volume(double vol);
    
    void set_client_ptr(Client* c); //To set the client to which the order corresponds
    Client* get_client_ptr();   //Get a pointer to the client who has issued the order
    
    void send_order_data(); //Send the data of the order to the client who made it. Ideally this function is called once an order is executed and a trade is made

protected:
    std::string symbol_;
    double price_;
    double volume_;
    TimePoint timePlaced_;
    bool isActive_;
    int id_;
    Client *client_ptr;
};

class Bid: public Order {
public:
    Bid();
    Bid(const std::string& symbol, double price, double volume);
    Bid(const Order& order);
    ~Bid() = default;
    /*
     * Comparator overloads sort by price-time priority.
     * If order1 has a better price than order2, then order1 is given priority
     * and order1 > order2.
     */
    bool operator<(const Bid& rhs) const;
    bool operator>(const Bid& rhs) const;
    std::string typeName() const;
};

class Ask: public Order {
public:
    Ask();
    Ask(const std::string& symbol, double price, double volume);
    Ask(const Order& order);
    ~Ask() = default;
    /*
     * Comparator overloads sort by price-time priority.
     * If order1 has a better price than order2, then order1 is given priority
     * and order1 > order2.
     */
    bool operator<(const Ask& rhs) const;
    bool operator>(const Ask& rhs) const;
    std::string typeName() const;
};

bool compatible_orders(Bid& b, Ask& a);   //Function to check whether these orders are compatible and a trade can be made

#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest/doctest.h"
TEST_CASE("test order initialization")
{
    Ask a;
    CHECK(a.symbol() == "AAPL");
    CHECK(a.price() == 0.0);
    CHECK(a.volume() == 0.0);

    // Check that error is raised when volume is negative
    CHECK_THROWS(Ask("AAPL", 10.0, 100.0));

    std::stringstream ss;
    ss << a;
    // CHECK(ss.str() == "foobar");
    CHECK(ss.str().find("Ask") == 0);

    Bid b {"AAPL", 10.0, 100.0};
    std::stringstream ss2;
    ss2 << b;
    CHECK(ss2.str().find("Bid") == 0);
    CHECK_THROWS(Bid("AAPL", 10.0, -100.0));
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
    CHECK(b1 < b2); // Better price
    CHECK(b2 > b1);
    CHECK(b2 > b3); // Better price

    // Check overloads for asks
    CHECK(a0 > a2); // Better price
    CHECK(a2 < a0);
    CHECK(a0 != a1);
    CHECK(a0 > a1); // Same price, but a0 placed earlier
}
#endif
