#include "matching_engine.hpp"
#include <sstream>


MatchingEngine::MatchingEngine():
buyOrders_(), sellOrders_(), orders_()
{}

void MatchingEngine::addOrder(const Order& order)
{
    if (order.isActive()) {
        if (order.typeName() == std::string("Bid")) {
            buyOrders_.push(order);
        } else if (order.typeName() == std::string("Ask")) {
            sellOrders_.push(order);
        }
        this->all_orders_.push_back(order);
    }
}

Bid MatchingEngine::top_buy(){
    return this->buyOrders_.top();   
}

Ask MatchingEngine::top_sell(){
    return this->sellOrders_.top();
}

void MatchingEngine::addOrder(const Bid& order)
{
    if (order.isActive()) {
        buyOrders_.push(order);
        this->all_orders_.push_back(order);
    }
}

void MatchingEngine::addOrder(const Ask& order)
{
    if (order.isActive()) {
        sellOrders_.push(order);
        this->all_orders_.push_back(order);
    }
}

void MatchingEngine::cancelOrder(Order& order)
{
    order.deactivate();
    // TODO: remove from heap
}

bool MatchingEngine::any_buy_orders(){
    return !(this->buyOrders_.empty());
}

bool MatchingEngine::any_sell_orders(){
    return !(this->sellOrders_.empty());
}

bool MatchingEngine::any_orders(){
    std::cout << this->sellOrders_.empty() << " " << this->buyOrders_.empty() << std::endl;
    return !(this->sellOrders_.empty() || this->buyOrders_.empty());
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



