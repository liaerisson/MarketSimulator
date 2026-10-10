#include <cstdint>

/**
 * @brief Immutable record of a single trade execution.
 *
 * A Trade is created when an incoming order matches with a resting order.
 * It records the participating orders and traders, the execution price,
 * and the quantity filled.
 */
struct Trade
{
    const std::uint64_t tradeId; // (Unique) ID of the trade.
    const std::uint64_t buyOrderId; // ID of the participating buy order.
    const std::uint64_t sellOrderId; // ID of the participating sell order.
    const std::uint64_t buyerTraderId; // ID of the buyer.
    const std::uint64_t sellerTraderId; // ID of the seller.
    const std::int64_t price; // Execution price of the trade.
    const std::uint64_t quantity; // Quantity fulfilled in the trade.
};

/**
 * @brief Compares two trade records for exact equality.
 *
 * @param left First trade to compare.
 * @param right Second trade to compare.
 * @return true if all trade fields are equal; otherwise false.
 */
inline bool operator== (const Trade &left, const Trade &right)
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