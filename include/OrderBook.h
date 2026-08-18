#pragma once

#include "Order.h"

#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <unordered_map>
#include <optional>

struct OrderLocation {
    Side side;
    std::int64_t price;
    std::list<Order>::iterator iterator;
};

class OrderBook {
    private:
        std::map<std::int64_t, std::list<Order>> sells; //price, list of orders with said price, lowest sell prices first
        std::map<std::int64_t, std::list<Order>, std::greater<std::int64_t>> buys; //price, list of orders with said price, highest buy prices first
        std::unordered_map<std::uint64_t, OrderLocation> orderLookup; //orderId, order location
        std::uint64_t orderId;
        std::uint64_t sequenceNumber;

        std::uint64_t getNewOrderId();
        
        std::uint64_t getNewSequenceNumber();
        
        void matchOrders(Order& newOrder);
        
        void removeOrder(std::uint64_t orderId);

    public:
        OrderBook();
        
        std::uint64_t addOrder(std::uint64_t traderId, Side side, std::int64_t price, std::uint64_t quantity);
        
        void cancelOrder(std::uint64_t orderId, std::uint64_t traderId);
        
        bool containsOrder(std::uint64_t orderId) const;

        std::optional<std::int64_t> getBestBid() const;
        std::optional<std::int64_t> getBestAsk() const;

        std::optional<std::uint64_t> getOrderQuantity(std::uint64_t orderId) const;
};