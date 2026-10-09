#include <cstdint>
#include "Order.h"
#include <stdexcept>

/**
 * @brief Ensures that an order's price is valid.
 * 
 * @param price Limit price of an order.
 * @return The validated price.
 * @throws std::invalid_argument If price is not positive.
 */
std::int64_t Order::validatePrice(std::int64_t price) {
    if(price <= 0) {
        throw std::invalid_argument("Price must be positive.");
    }
    
    return price;
}

/**
 * @brief Ensures that an order's quantity is valid.
 * 
 * @param quantity Quantity requested for an order.
 * @return The validated quantity.
 * @throws std::invalid_argument If quantity is not positive.
 */
std::uint64_t Order::validateQuantity(std::uint64_t quantity) {
    if(quantity == 0) {
        throw std::invalid_argument("Quantity must be >0.");
    }

    return quantity;
}

/**
 * @brief Initializes a new order with the given order ID, trader ID, side, price, quantity, and sequence number.
 * 
 * @param orderId Order ID of the order.
 * @param traderId The trader submitting the order.
 * @param side Whether the order is a buy or sell.
 * @param price Limit price of the order.
 * @param quantity Number of units in the order.
 * @param sequenceNumber Sequence number of the order.
 * 
 * @throws std::invalid_argument If quantity is not valid.
 * @throws std::invalid_argument If price is not valid.
 */
Order::Order(std::uint64_t orderId, 
    std::uint64_t traderId, 
    Side side, 
    std::int64_t price, 
    std::uint64_t quantity, 
    std::uint64_t sequenceNumber
) 
    : orderId(orderId), 
    traderId(traderId), 
    price(validatePrice(price)), 
    side(side), 
    quantity(validateQuantity(quantity)), 
    sequenceNumber(sequenceNumber)
{
    
}

/**
 * @brief Reduces the quantity in a resting order by the amount fulfilled by a trade.
 * 
 * @param purchased The quantity fulfilled, which is reduced from the standing quantity.
 * @throws std::invalid_argument If the fill quantity is zero or exceeds
 * the order's remaining quantity.
 * 
 * @note This validation protects the Order invariant even though matchOrders() 
 * normally ensures a valid fill quantity using std::min().
 */
void Order::reduceQuantity(std::uint64_t purchased) {
    if(purchased == 0 || purchased > quantity) {
        throw std::invalid_argument("Invalid amount to fill.");
    }

    quantity -= purchased;
}

/**
 * @brief Gets the traderId of the order.
 * @return The trader ID of the order.
 */
std::uint64_t Order::getTraderId() const {
    return traderId;
}

/**
 * @brief Gets the side of the order.
 * @return The side of the order.
 */
Side Order::getSide() const {
    return side;
}

/**
 * @brief Gets the price of the order.
 * @return The price of the order.
 */
std::int64_t Order::getPrice() const {
    return price;
}

/**
 * @brief Gets the quantity of the order.
 * @return The current quantity of the order.
 */
std::uint64_t Order::getQuantity() const {
    return quantity;
}

/**
 * @brief Gets the order ID of the order.
 * @return The order ID of the order.
 */
std::uint64_t Order::getOrderId() const {
    return orderId;
}

/**
 * @brief Gets the sequence number of the order.
 * @return The sequenceNumber of the order.
 */
std::uint64_t Order::getSequenceNumber() const {
    return sequenceNumber;
}