#pragma once

#include "Order.h"

#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <unordered_map>

struct OrderLocation {
    Side side;
    std::int64_t price;
    std::list<Order>::iterator iterator;
};

class OrderBook {
    private:
        std::map<std::int64_t, std::list<Order>> sells; //price, list of orders with said price
        std::map<std::int64_t, std::list<Order>, std::greater<std::int64_t>> buys; //price, list of orders with said price
        std::unordered_map<std::uint64_t, OrderLocation> orderLookup; //orderId, order location
        std::uint64_t orderId;
        int sequenceNumber;

        std::uint64_t getNewOrderId();
        int getNewSequenceNumber();

    public:
        OrderBook();
        
        void addOrder(std::uint64_t traderId, std::int64_t price, Side side, std::uint64_t quantity);
        
        void cancelOrder(std::uint64_t orderId, std::uint64_t traderId);
        
        bool containsOrder(std::uint64_t orderId) const;
};