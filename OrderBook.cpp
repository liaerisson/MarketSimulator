#include "OrderBook.h"

#include <iterator>
#include <stdexcept>

OrderBook::OrderBook() 
    : orderId(1), 
    sequenceNumber(1) 
{
}

std::uint64_t OrderBook::getNewOrderId() {
    return orderId++;
}

int OrderBook::getNewSequenceNumber() {
    return sequenceNumber++;
}



void OrderBook::addOrder(std::uint64_t traderId, std::int64_t price, Side side, std::uint64_t quantity) {
    std::uint64_t order = getNewOrderId();
    int sequenceNum = getNewSequenceNumber();
    Order newOrder = Order(order, traderId, side, price, quantity, sequenceNum);
    
    OrderLocation location;
    location.price = price;
    location.side = side;

    if(side == Side::BUY) {
        //group references the actual list in the map, so manipulations still apply to it
        auto& group = buys[price];
        group.push_back(newOrder);
        location.iterator = std::prev(group.end());
    
    } else if(side == Side::SELL) {
        auto& group = sells[price];
        group.push_back(newOrder);
        location.iterator = std::prev(group.end());
    }

    orderLookup.insert({order, location});
}

void OrderBook::cancelOrder(std::uint64_t orderId, std::uint64_t traderId) {
    auto it = orderLookup.find(orderId);
    if(it == orderLookup.end()) {
        throw std::invalid_argument("Order ID does not exist."); 
    }

    const OrderLocation& location = it->second;
    const Order& currentOrder = *(it->second.iterator);

    if(currentOrder.getTraderId() != traderId) {
        throw std::invalid_argument("Trader does not own this order.");
    }

    if(location.side == Side::BUY) {
        auto levelIt = buys.find(location.price);
        levelIt->second.erase(location.iterator);

        if(levelIt->second.empty()) {
            buys.erase(levelIt);
        }
    } else {
        auto levelIt = sells.find(location.price);
        levelIt->second.erase(location.iterator);
        
        if(levelIt->second.empty()) {
            sells.erase(levelIt);
        }
    }

    orderLookup.erase(it);
}

bool OrderBook::containsOrder(std::uint64_t orderId) const {
    return orderLookup.contains(orderId);
}