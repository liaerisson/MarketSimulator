#include "OrderBook.h"

#include <iterator>
#include <stdexcept>
#include <algorithm>

OrderBook::OrderBook() 
    : orderId(1), 
    sequenceNumber(1) 
{
}

std::uint64_t OrderBook::getNewOrderId() {
    return orderId++;
}

std::uint64_t OrderBook::getNewSequenceNumber() {
    return sequenceNumber++;
}

void OrderBook::matchOrders(Order& newOrder) {
    Side orderSide = newOrder.getSide();
    std::int64_t orderPrice = newOrder.getPrice();

    if(orderSide == Side::BUY) {
        while(newOrder.getQuantity() > 0 && !sells.empty()) {
            //an iterator starting at the top of the sells list
            auto levelIt = sells.begin();

            if(levelIt->first > orderPrice) {
                break;
            }

            Order& currentOrder = levelIt->second.front();
            std::uint64_t newQuantity = newOrder.getQuantity();
            std::uint64_t currentQuantity = currentOrder.getQuantity();

            std::uint64_t fillQuantity = std::min(newQuantity, currentQuantity);
            
            currentOrder.reduceQuantity(fillQuantity);
            newOrder.reduceQuantity(fillQuantity);

            if(currentOrder.getQuantity() == 0) { 
                removeOrder(currentOrder.getOrderId());
            }
        }
  
        //check first item in sell list, if price <= buy offer, then check values
        //if number in sell list trader A < amount wanted to buy, clear order A, then start B etc...
    } else {
        while(newOrder.getQuantity() > 0 && !buys.empty()) {
            //an iterator starting at the top of the sells list
            auto levelIt = buys.begin();

            if(levelIt->first < orderPrice) {
                break;
            }

            Order& currentOrder = levelIt->second.front();
            std::uint64_t newQuantity = newOrder.getQuantity();
            std::uint64_t currentQuantity = currentOrder.getQuantity();

            std::uint64_t fillQuantity = std::min(newQuantity, currentQuantity);
            
            currentOrder.reduceQuantity(fillQuantity);
            newOrder.reduceQuantity(fillQuantity);

            if(currentOrder.getQuantity() == 0) { 
                removeOrder(currentOrder.getOrderId());
            }
        }
    }
}


void OrderBook::addOrder(std::uint64_t traderId, Side side, std::int64_t price, std::uint64_t quantity) {
    if(price <= 0) {
        throw std::invalid_argument("Price must be positive.");
    }

    if(quantity == 0) {
        throw std::invalid_argument("Quantity must be >0.");
    }
    
    std::uint64_t order = getNewOrderId();
    std::uint64_t sequenceNum = getNewSequenceNumber();
    Order newOrder = Order(order, traderId, side, price, quantity, sequenceNum);
    
    matchOrders(newOrder);
    if(newOrder.getQuantity() == 0) {
        return;
    }

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

    removeOrder(orderId);
}

void OrderBook::removeOrder(std::uint64_t orderId) {
    auto it = orderLookup.find(orderId);
    const OrderLocation& location = it->second;
    const Order& currentOrder = *(it->second.iterator);
    
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