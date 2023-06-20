#include "order.hpp"

Order::Order()
: symbol_("AAPL"), price_(0.0), volume_(0.0),
  timePlaced_(std::chrono::system_clock::now()), isActive_(true)
{}

Order::Order(const std::string& symbol, double price, double volume)
: symbol_(symbol), price_(price), volume_(volume),
  timePlaced_(std::chrono::system_clock::now()), isActive_(true)
{}

std::string Order::symbol() const
{
    return symbol_;
}

int Order::id() const
{
    return id_;
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
    return symbol_ == rhs.symbol_ && price_ == rhs.price_ && volume_ == rhs.volume_ && timePlaced_ == rhs.timePlaced_;
}

bool Order::operator!=(const Order& rhs) const
{
    return !(*this == rhs);
}

std::string Order::typeName() const
{
    return "Order";
}

Bid::Bid()
: Order() {}

Bid::Bid(const std::string& symbol, double price, double volume)
: Order(symbol, price, volume) {
    if (volume < 0) {
        throw std::invalid_argument("Bid volume must be positive.");
    }
}

Bid::Bid(const Order& order) : Bid(order.symbol(), order.price(), order.volume()) {}

bool Bid::operator<(const Bid& rhs) const
{
    if (price_ == rhs.price_) {
        // If prices are equal, then the order placed first has priority.
        return timePlaced_ > rhs.timePlaced_;
    } else {
        return price_ < rhs.price_;
    }
}

bool Bid::operator>(const Bid& rhs) const
{
    if (price_ == rhs.price_) {
        return timePlaced_ < rhs.timePlaced_;
    } else {
        return price_ > rhs.price_;
    }
}

std::string Bid::typeName() const
{
    return "Bid";
}

Ask::Ask()
: Order() {}

Ask::Ask(const std::string& symbol, double price, double volume)
: Order(symbol, price, volume) {
    if (volume > 0) {
        throw std::invalid_argument("Ask volume must be negative.");
    }
}

Ask::Ask(const Order& order) : Ask(order.symbol(), order.price(), order.volume()) {}

bool Ask::operator<(const Ask& rhs) const
{
    if (price_ == rhs.price_) {
        return timePlaced_ > rhs.timePlaced_;
    } else {
        // Lower price has priority for asks.
        return price_ > rhs.price_;
    }
}

bool Ask::operator>(const Ask& rhs) const
{
    if (price_ == rhs.price_) {
        return timePlaced_ < rhs.timePlaced_;
    } else {
        // Lower price has priority for asks.
        return price_ < rhs.price_;
    }
}

std::string Ask::typeName() const
{
    return "Ask";
}

std::ostream& operator<<(std::ostream& s, const Order& o)
{
    std::stringstream ss;
    ss << order.typeName();
    ss << "(symbol: " << order.symbol() << ", price: " << order.price() << ", volume: ";
    ss << order.volume() << ", timePlaced: " << order.timePlaced().time_since_epoch().count();
    ss << ", isActive: " << order.isActive() << ")";
    os << ss.str();
    return os;
}