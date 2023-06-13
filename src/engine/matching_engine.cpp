#include "matching_engine.hpp"


MatchingEngine::MatchingEngine():
buyOrders_(), sellOrders_(), orders_()
{}

void MatchingEngine::addOrder(const Order& order)
{
    if (order.isBuy()) {
        buyOrders_.push(order);
    } else if (order.isSell()) {
        sellOrders_.push(order);
    }
    orders_.insert({order.id(), order});
}

void MatchingEngine::cancelOrder(Order& order)
{
    //deactivate is a non const function, so we cannot have order has a const input here
    order.deactivate();
    // TODO
}

void MatchingEngine::printOrderBook() const
{
    std::cout << "Buy Orders:" << std::endl;
    buyOrders_.print();
    std::cout << "Sell Orders:" << std::endl;
    sellOrders_.print();
}



