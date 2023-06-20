#include "matching_engine.h"
#include <sstream>


MatchingEngine::MatchingEngine():
buyOrders_(), sellOrders_(), orders_()
{}

void MatchingEngine::addOrder(const Order& order)
{
    if (order.isActive()) {
        if (order.typeName() == "Bid") {
            buyOrders_.push(order);
        } else if (order.typeName() == "Ask") {
            sellOrders_.push(order);
        }
    }
}

void MatchingEngine::cancelOrder(const Order& order)
{
    order.deactivate();
    // TODO: remove from heap
}

std::string MatchingEngine::printBuyOrders() const
{
    std::stringstream ss;
    for (const auto& order : buyOrders_) {
        ss << order << "\n";
    }
    return ss.str();
}

std::string MatchingEngine::printSellOrders() const
{
    std::stringstream ss;
    for (const auto& order : sellOrders_) {
        ss << order << "\n";
    }
    return ss.str();
}

ostream& operator<<(ostream& os, const MatchingEngine& me);
{
    std::sstream ss;
    ss << "Buy Orders:" << "\n" << me.printBuyOrders();
    ss << "Sell Orders:" << "\n" << me.printSellOrders();
    os << ss.str();
    return os;
}



