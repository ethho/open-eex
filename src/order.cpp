#include "order.h"

Order::Order():
symbol_("AAPL"), price_(0.0), volume_(0.0), isActive_(true)
{}

Order::Order(const std::string& symbol, double price, double volume):
symbol_(symbol), price_(price), volume_(volume), isActive_(true)
{}

std::string Order::symbol() const
{
    return symbol_;
}

double Order::price() const
{
    return price_;
}

double Order::volume() const
{
    return volume_;
}

bool Order::isActive() const
{
    return isActive_;
}

void Order::deactivate()
{
    isActive_ = false;
}

bool Order::isSell() const
{
    return volume_ < 0.0;
}

bool Order::isBuy() const
{
    return volume_ > 0.0;
}

bool Order::operator<(const Order& rhs) const
{
    return price_ < rhs.price_;
}

bool Order::operator>(const Order& rhs) const
{
    return price_ > rhs.price_;
}

bool Order::operator==(const Order& rhs) const
{
    return symbol_ == rhs.symbol_ && price_ == rhs.price_ && volume_ == rhs.volume_;
}


std::ostream& operator<<(std::ostream& os, Order const & order) {
    os << "Order(symbol=" << order.symbol() << ", price=" << order.price();
    os << ", volume=" << order.volume() << ", isActive=" << order.isActive() << ")";
    return os;
}
