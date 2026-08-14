#include <cstdint>
#include <string>
#include <iostream>
#include "Order.h"
#include <stdexcept>


std::int64_t Order::validatePrice(std::int64_t price) {
    if(price <= 0) {
        throw std::invalid_argument("Price must be positive.");
    }
    
    return price;
}

std::uint64_t Order::validateQuantity(std::uint64_t quantity) {
    if(quantity == 0) {
        throw std::invalid_argument("Quantity must be >0.");
    }

    return quantity;
}

Order::Order(std::uint64_t orderId, 
    std::uint64_t traderId, 
    Side side, 
    std::int64_t price, 
    std::uint64_t quantity, 
    int sequenceNumber
) 
    : orderId(orderId), 
    traderId(traderId), 
    price(validatePrice(price)), 
    side(side), 
    quantity(validateQuantity(quantity)), 
    sequenceNumber(sequenceNumber)
{
    
}

void Order::reduceQuantity(std::uint64_t purchased) {
    if(purchased == 0 || purchased > quantity) {
        throw std::invalid_argument("Invalid amount to fill.");
    }

    quantity -= purchased;
}

std::uint64_t Order::getTraderId() const {
    return traderId;
}

Side Order::getSide() const {
    return side;
}

std::int64_t Order::getPrice() const {
    return price;
}

std::uint64_t Order::getQuantity() const {
    return quantity;
}

std::uint64_t Order::getOrderId() const {
    return orderId;
}
