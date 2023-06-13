#include "order.h"

Order::Order():
symbol_("AAPL"), price_(0.0), volume_(0.0), isActive_(true)
{
    timePlaced_ = std::chrono::system_clock::now();
}

Order::Order(const std::string& symbol, double price, double volume):
symbol_(symbol), price_(price), volume_(volume), isActive_(true)
{
    timePlaced_ = std::chrono::system_clock::now();
}

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

TimePoint Order::timePlaced() const
{
    return timePlaced_;
}

bool Order::isActive() const
{
    return isActive_;
}

void Order::deactivate()
{
    isActive_ = false;
}

bool Order::operator==(const Order& rhs) const
{
    return symbol_ == rhs.symbol_ && price_ == rhs.price_ && volume_ == rhs.volume_;
}

bool Order::operator!=(const Order& rhs) const
{
    return !(*this == rhs);
}

bool Bid::operator<(const Bid& rhs) const
{
    return price_ < rhs.price_;
}

bool Bid::operator>(const Bid& rhs) const
{
    return price_ > rhs.price_;
}

bool Bid::operator==(const Bid& rhs) const
{
    return symbol_ == rhs.symbol_ && price_ == rhs.price_ && volume_ == rhs.volume_;
}

bool Bid::operator!=(const Bid& rhs) const
{
    return !(*this == rhs);
}

bool Ask::operator<(const Ask& rhs) const
{
    // Reversed because asks with lower price have higher priority.
    return price_ > rhs.price_;
}

bool Ask::operator>(const Ask& rhs) const
{
    return price_ < rhs.price_;
}

std::ostream& operator<<(std::ostream& os, Order const & order) {
    std::stringstream ss;
    ss << decltype(order)::name() << "(";
    ss << "(symbol=" << order.symbol() << ", price=" << order.price();
    ss << ", volume=" << order.volume() << ", isActive=" << order.isActive();
    ss << ", timePlaced=" << std::chrono::system_clock::to_time_t(order.timePlaced());
    ss << ")";
    os << ss.str();
    return os;
}
