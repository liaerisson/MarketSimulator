#pragma once

#include "Order.h"
#include <cstdint>

struct GeneratedOrder
{
    std::uint64_t traderId;       // ID of the trader making the order
    Side side;                    // Side of the incoming order
    std::int64_t price;           // Limit price of the order
    std::uint64_t quantity;       // Quantity requested in the order
};