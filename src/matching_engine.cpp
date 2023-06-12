#include "matching_engine.h"


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

void MatchingEngine::cancelOrder(const Order& order)
{
    order.deactivate();
    // TODO
}

void MatchingEngine::printOrderBook() const
{
    std::cout << "Buy Orders:" << "\n" << buyOrders_ << std::endl;
    std::cout << "Sell Orders:" << "\n" << buyOrders_ << std::endl;
}



