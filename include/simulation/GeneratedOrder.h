#pragma once

#include "Order.h"
#include <cstdint>

/**
 * @brief Representation of an order before submission to the OrderBook.
 *
 * GeneratedOrder stores only the inputs needed by OrderBook::addOrder().
 * It is used to create deterministic benchmark workloads independently of
 * the OrderBook's internal ID and sequence-number generation.
 */
struct GeneratedOrder
{
    std::uint64_t traderId;       // ID of the trader making the order
    Side side;                    // Side of the incoming order
    std::int64_t price;           // Limit price of the order
    std::uint64_t quantity;       // Quantity requested in the order
};