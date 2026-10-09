#include <cstdint>

#include "OrderBook.h"
#include <vector>

/**
 * @brief Frozen representation of a single resting order.
 */
struct OrderSnapshot {
    std::uint64_t sequenceNumber; // Arrival sequence used to ensure time priority.
    std::uint64_t orderId; // Unique ID of the resting order
    std::uint64_t traderId; // ID of the trader who submitted the order
    std::uint64_t quantity; // Remaining quantity of the resting order
};

/**
 * @brief Frozen representation of one price level in the order book.
 */
struct PriceLevelSnapshot {
    std::int64_t price; // Price shared by all orders at this level
    std::vector<OrderSnapshot> orders; // Resting orders in FIFO priority order
};

/**
 * @brief Frozen representation of the current resting-order state of the order book.
 */
struct BookSnapshot {
    std::vector<PriceLevelSnapshot> buyPrices; // Buy-side price levels from highest to lowest price
    std::vector<PriceLevelSnapshot> sellPrices; // Sell-side price levels from lowest to highest price
};