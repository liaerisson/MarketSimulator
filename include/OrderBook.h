#pragma once

#include "Order.h"
#include "Trade.h"
#include "BookSnapshot.h"

#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <unordered_map>
#include <optional>
#include <vector>

struct OrderLocation
{
    Side side;
    std::int64_t price;

    // Orders at the same price are stored in arrival order.
    // std::list gives stable iterators so orderLookup can point directly to resting orders.
    std::list<Order>::iterator iterator;
};

class OrderBook
{
private:
    // Price levels are ordered ascending so sells.begin() is always the best sell.
    std::map<std::int64_t, std::list<Order>> sells;
    // Price levels are ordered descending so buys.begin() is always the best bid.
    std::map<std::int64_t, std::list<Order>, std::greater<std::int64_t>> buys;

    std::vector<Trade> trades;

    std::unordered_map<std::uint64_t, OrderLocation> orderLookup; // orderId, order location
    std::uint64_t orderId;
    std::uint64_t sequenceNumber;
    std::uint64_t tradeId;

    std::uint64_t getNewOrderId();

    std::uint64_t getNewSequenceNumber();

    std::uint64_t getNewTradeId();

    void matchOrders(Order &newOrder);

    void removeOrder(std::uint64_t orderId);

public:
    OrderBook();

    std::uint64_t addOrder(std::uint64_t traderId, Side side, std::int64_t price, std::uint64_t quantity);

    void cancelOrder(std::uint64_t orderId, std::uint64_t traderId);

    bool containsOrder(std::uint64_t orderId) const;

    std::optional<std::int64_t> getBestBid() const;
    std::optional<std::int64_t> getBestAsk() const;

    std::optional<std::uint64_t> getOrderQuantity(std::uint64_t orderId) const;

    std::optional<Trade> getLastTrade() const;
    const std::vector<Trade> &getTrades() const;

    BookSnapshot getSnapshot() const;
};