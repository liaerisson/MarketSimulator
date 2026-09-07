#include <cstdint>

struct Trade {
    const std::uint64_t tradeId;
    const std::uint64_t buyOrderId;
    const std::uint64_t sellOrderId;
    const std::uint64_t buyerTraderId;
    const std::uint64_t sellerTraderId;
    const std::int64_t price;
    const std::uint64_t quantity;
};