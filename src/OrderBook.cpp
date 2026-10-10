#include "OrderBook.h"

#include <iterator>
#include <stdexcept>
#include <algorithm>

/**
 * @brief Initialize a new OrderBook
 * @return A new, empty orderbook whos orderId, sequenceNumber, and tradeId counts begin at 1.
 */
OrderBook::OrderBook()
    : orderId(1),
      sequenceNumber(1),
      tradeId(1)
{
}

/**
 * @brief Get a new order ID
 * @return A new order ID 1 higher than the previous.
 */
std::uint64_t OrderBook::getNewOrderId()
{
    return orderId++;
}

/**
 * @brief Get a new sequence number
 * @return A new sequence number  1 higher than the previous.
 */
std::uint64_t OrderBook::getNewSequenceNumber()
{
    return sequenceNumber++;
}

/**
 * @brief Get a new trade ID
 * @return A new trade ID  1 higher than the previous.
 */
std::uint64_t OrderBook::getNewTradeId()
{
    return tradeId++;
}

/**
 * @brief Matches and trades incoming orders with crossing resting orders.
 *
 * Matches the incoming order repeatedly against the best-priced resting orders until the incoming quantity
 * is fully filled, the opposite side of the book is empty, or the best available price no longer crosses
 * the incoming order's limit price.
 *
 * Within a price level, orders are matched in FIFO order. Each fill creates a Trade at the resting
 * order's price. Fully filled resting orders are removed from the order book.
 *
 * @param newOrder The incoming order to match. Its remaining quantity is reduced as fills occur.
 * @note A buy order crosses when its limit price is greater than or equal to the best ask.
 * A sell order crosses when its limit price is less than or equal to the best bid.
 * @note This method acts on the trades and adjusts the quantities of the orders. Meanwhile, addOrder() and removeOrder()
 * handle the subsequent process.
 */
void OrderBook::matchOrders(Order &newOrder)
{
    Side orderSide = newOrder.getSide();
    std::int64_t orderPrice = newOrder.getPrice();

    if (orderSide == Side::BUY)
    {
        while (newOrder.getQuantity() > 0 && !sells.empty())
        {
            // The first sell price level is the best ask because sells are sorted ascending.
            auto levelIt = sells.begin();

            // Stop when the best ask exceeds the incoming buy's limit price.
            if (levelIt->first > orderPrice) // buy can no longer cross
            {
                break;
            }

            // Match the oldest resting order at this price level to preserve FIFO priority.
            Order &sellOrder = levelIt->second.front();

            std::uint64_t newQuantity = newOrder.getQuantity();
            std::uint64_t currentQuantity = sellOrder.getQuantity();

            // A fill can consume no more than the remaining quantity of the smaller order
            std::uint64_t fillQuantity = std::min(newQuantity, currentQuantity);

            // Record the trade at the resting sell order's price.
            trades.push_back(Trade{
                getNewTradeId(),
                newOrder.getOrderId(),
                sellOrder.getOrderId(),
                newOrder.getTraderId(),
                sellOrder.getTraderId(),
                sellOrder.getPrice(),
                fillQuantity});

            sellOrder.reduceQuantity(fillQuantity);
            newOrder.reduceQuantity(fillQuantity);

            // Remove fully filled resting orders from the book and lookup table.
            if (sellOrder.getQuantity() == 0)
            {
                removeOrder(sellOrder.getOrderId());
            }
        }
    }
    else
    {
        while (newOrder.getQuantity() > 0 && !buys.empty())
        {
            auto levelIt = buys.begin();

            if (levelIt->first < orderPrice) // sell can no longer cross
            {
                break;
            }

            Order &buyOrder = levelIt->second.front();
            std::uint64_t newQuantity = newOrder.getQuantity();
            std::uint64_t currentQuantity = buyOrder.getQuantity();

            std::uint64_t fillQuantity = std::min(newQuantity, currentQuantity);

            trades.push_back(Trade{
                getNewTradeId(),
                buyOrder.getOrderId(),
                newOrder.getOrderId(),
                buyOrder.getTraderId(),
                newOrder.getTraderId(),
                buyOrder.getPrice(),
                fillQuantity});

            buyOrder.reduceQuantity(fillQuantity);
            newOrder.reduceQuantity(fillQuantity);

            if (buyOrder.getQuantity() == 0)
            {
                removeOrder(buyOrder.getOrderId());
            }
        }
    }
}

/**
 * @brief Adds a new order to the order book.
 *
 * When added, the order is matched against the current resting orders.
 * Any unused quantity from the order is added to the appropriate price level.
 *
 * @param traderId The trader submitting the order.
 * @param side Whether the order is a buy or sell.
 * @param price Limit price of the order.
 * @param quantity Number of units in the order.
 * @return The unique order ID from getOrderId() assigned to the submission.
 * @throws std::invalid_argument If price or quantity is invalid.
 */
std::uint64_t OrderBook::addOrder(std::uint64_t traderId, Side side, std::int64_t price, std::uint64_t quantity)
{
    // Checking base conditions: the price and quantity within an order must be greater than 0
    if (price <= 0)
    {
        throw std::invalid_argument("Price must be positive.");
    }

    if (quantity == 0)
    {
        throw std::invalid_argument("Quantity must be >0.");
    }

    // If the order if valid, then is recieves a new order ID and sequence number
    std::uint64_t orderId = getNewOrderId();
    std::uint64_t sequenceNum = getNewSequenceNumber();

    // The literal order is then created, but it is not yet in the book
    Order newOrder = Order(orderId, traderId, side, price, quantity, sequenceNum);

    // Call matchOrders, which settles out any resting orders that can be filled by the incoming order
    // If the new order is fully settled out, then its orderId is returned but the order is never logged in the book
    matchOrders(newOrder);
    if (newOrder.getQuantity() == 0)
    {
        return orderId;
    }

    // The order location is assigned to make it easier to retrieve later on
    OrderLocation location;
    location.price = price;
    location.side = side;

    // Add the remaining order to the back of its price level to preserve FIFO priority
    if (side == Side::BUY)
    {
        auto &group = buys[price];
        group.push_back(newOrder);
        location.iterator = std::prev(group.end());
    }
    else if (side == Side::SELL)
    {
        auto &group = sells[price];
        group.push_back(newOrder);
        location.iterator = std::prev(group.end());
    }

    // Following insrtion, the orderId is returned to the trader for future reference
    orderLookup.insert({orderId, location});
    return orderId;
}

/**
 * @brief Cancels orders from the trader/client side
 *
 * Cancelling allows a trader to cancel their order follow its initial trades with the resting orders. Whatever's left is removed.
 * CancelOrder validates the request of the trader, then calls removeOrder to remove the order from the order book and order locations.
 *
 * @param orderId order ID of the order being cancelled
 * @param traderId ID of the trader who owns the order
 * @throws std::invalid_argument If order does not exist in the book. This means the order either never existed or was fully fulfilled by that point.
 * @throws std::invalid_argument If the trader does not own the order they are attempting to cancel.
 * @see removeOrder
 */
void OrderBook::cancelOrder(std::uint64_t orderId, std::uint64_t traderId)
{
    auto it = orderLookup.find(orderId);
    if (it == orderLookup.end())
    {
        throw std::invalid_argument("Order ID does not exist.");
    }

    const OrderLocation &location = it->second;
    const Order &currentOrder = *(it->second.iterator);

    if (currentOrder.getTraderId() != traderId)
    {
        throw std::invalid_argument("Trader does not own this order.");
    }

    removeOrder(orderId);
}

/**
 * @brief Removes an order from the order book
 * This allows a trader to remove their order from the book follow its initial trades with the resting orders.
 *
 * @param orderId order ID of the order being cancelled
 */
void OrderBook::removeOrder(std::uint64_t orderId)
{
    // Get the location of the corresponding orderId by (key, value) retrieval
    auto it = orderLookup.find(orderId);
    const OrderLocation &location = it->second;

    // Identifies the side and price level of the order being removed
    // Locate the order within the price level using location.iterator (the prestored iterator of its location)
    if (location.side == Side::BUY)
    {
        auto levelIt = buys.find(location.price);
        levelIt->second.erase(location.iterator);

        // If there are no orders left at the price level of the removed order, remove the entire price level from the buys map
        if (levelIt->second.empty())
        {
            buys.erase(levelIt);
        }
    }
    else
    {
        auto levelIt = sells.find(location.price);
        levelIt->second.erase(location.iterator);

        // If there are no orders left at the price level of the removed order, remove the entire price level from the sells map
        if (levelIt->second.empty())
        {
            sells.erase(levelIt);
        }
    }

    // Remove the iterator from orderLookup
    orderLookup.erase(it);
}

/**
 * @brief Checks whether the book contains a specific order.
 * @return bool representing whether the specified order is in the book
 */
bool OrderBook::containsOrder(std::uint64_t orderId) const
{
    return orderLookup.contains(orderId);
}

/**
 * @brief Get the best going bid price
 * @return The highest resting buy order price, or a null optional if there are no resting buy orders
 */
std::optional<std::int64_t> OrderBook::getBestBid() const
{
    if (buys.empty())
    {
        return std::nullopt; // null optional
    }

    return buys.begin()->first; // highest buy price
}

/**
 * @brief Get the best going ask price
 * @return The lowest resting sell order price, or a null optional if there are no resting sell orders
 */
std::optional<std::int64_t> OrderBook::getBestAsk() const
{
    if (sells.empty())
    {
        return std::nullopt;
    }

    return sells.begin()->first; // lowest sell price
}

/**
 * @brief Get the quantity request in the specified order
 *
 * @param orderId The order ID of which the quantity is needed.
 * @return The highest resting buy order price, or a null optional if there are no resting buy orders
 */
std::optional<std::uint64_t> OrderBook::getOrderQuantity(std::uint64_t orderId) const
{
    auto it = orderLookup.find(orderId);

    if (it == orderLookup.end())
    {
        return std::nullopt;
    }

    return it->second.iterator->getQuantity();
}

/**
 * @brief Get the most recently occurred trade
 * @return The last trade that occurred, or a null optional if there is no trade history
 */
std::optional<Trade> OrderBook::getLastTrade() const
{
    if (trades.empty())
    {
        return std::nullopt;
    }

    return trades.back(); // most recent trade
}

/**
 * @brief Get the past trades
 * @return Full list of previous trades
 */
const std::vector<Trade> &OrderBook::getTrades() const
{
    return trades;
}


BookSnapshot OrderBook::getSnapshot() const {
    BookSnapshot book;

    for (const auto& level : buys) {
        PriceLevelSnapshot priceLevel;
        priceLevel.price = level.first;

        for (const auto& order : level.second) {
            OrderSnapshot snapshot = {order.getSequenceNumber(), order.getOrderId(), order.getTraderId(), order.getQuantity()};
            priceLevel.orders.push_back(snapshot);
        }

        book.buyPrices.push_back(priceLevel);
    }

    for (const auto& level : sells) {
        PriceLevelSnapshot priceLevel;
        priceLevel.price = level.first;

        for (const auto& order : level.second) {
            OrderSnapshot snapshot = {order.getSequenceNumber(), order.getOrderId(), order.getTraderId(), order.getQuantity()};
            priceLevel.orders.push_back(snapshot);
        }

        book.sellPrices.push_back(priceLevel);
    }

    return book;
}