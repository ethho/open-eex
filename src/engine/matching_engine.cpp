#include "matching_engine.hpp"


MatchingEngine::MatchingEngine():
buyOrders_(), sellOrders_(), orders_()
{}

void MatchingEngine::addOrder(const Ask& ask)
{
    buyOrders_.push(ask);
    orders_.insert({ask.id(), ask});
}

void MatchingEngine::addOrder(const Bid& bid)
{
    sellOrders_.push(bid);
    orders_.insert({bid.id(), bid});
}

void MatchingEngine::cancelOrder(Order& order)
{
    //deactivate is a non const function, so we cannot have order has a const input here
    order.deactivate();
    // TODO
}

// void MatchingEngine::printOrderBook() const
// {
//     std::cout << "Buy Orders:" << "\n" << buyOrders_ << std::endl;
//     std::cout << "Sell Orders:" << "\n" << buyOrders_ << std::endl;
// }



