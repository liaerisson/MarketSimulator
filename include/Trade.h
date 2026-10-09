#include <cstdint>

struct Trade
{
    const std::uint64_t tradeId;
    const std::uint64_t buyOrderId;
    const std::uint64_t sellOrderId;
    const std::uint64_t buyerTraderId;
    const std::uint64_t sellerTraderId;
    const std::int64_t price;
    const std::uint64_t quantity;
};

inline bool operator == (const Trade &left, const Trade &right)
{
    return (left.tradeId == right.tradeId && 
        left.buyOrderId == right.buyOrderId &&
        left.sellOrderId == right.sellOrderId &&
        left.buyerTraderId == right.buyerTraderId &&
        left.sellerTraderId == right.sellerTraderId &&
        left.price == right.price &&
        left.quantity == right.quantity
    );
}