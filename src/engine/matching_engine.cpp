#include "matching_engine.hpp"
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
        this->all_orders_.push_back(order);
    }
}

void MatchingEngine::cancelOrder(Order& order)
{
    order.deactivate();
    // TODO: remove from heap
}

// std::string MatchingEngine::printBuyOrders() const
// {
//     std::stringstream ss;
//     for (const auto& order : buyOrders_) {
//         ss << order << "\n";
//     }
//     return ss.str();
// }

// std::string MatchingEngine::printSellOrders() const
// {
//     std::stringstream ss;
//     for (const auto& order : sellOrders_) {
//         ss << order << "\n";
//     }
//     return ss.str();
// }

std::string MatchingEngine::printOrders() const
{
    std::stringstream ss;
    for (const Order& o : this->all_orders_){
        ss << o << "\n";
    }
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const MatchingEngine& me)
{
    std::stringstream ss;
    ss << "All orders: \n" << me.printOrders();
    //ss << "Buy Orders:" << "\n" << me.printBuyOrders();
    //ss << "Sell Orders:" << "\n" << me.printSellOrders();
    os << ss.str();
    return os;
}



