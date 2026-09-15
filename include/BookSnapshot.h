#include <cstdint>

#include "OrderBook.h"
#include <vector>

struct OrderSnapshot {
    std::uint64_t sequenceNumber;
    std::uint64_t orderId;
    std::uint64_t traderId;
    std::uint64_t quantity;
};

struct PriceLevelSnapshot {
    std::int64_t price;
    std::vector<OrderSnapshot> orders;
};

struct BookSnapshot {
    std::vector<PriceLevelSnapshot> buyPrices;
    std::vector<PriceLevelSnapshot> sellPrices;
};