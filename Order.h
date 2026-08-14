#pragma once
#include <cstdint>

enum class Side {
    BUY, 
    SELL
};

class Order {
    private:
        const std::uint64_t orderId;
        const std::uint64_t traderId;
        const std::int64_t price;
        const Side side;
        std::uint64_t quantity;
        const int sequenceNumber;

        static std::int64_t validatePrice(std::int64_t price);
        static std::uint64_t validateQuantity(std::uint64_t quantity);

    public:
        Order(
            std::uint64_t orderId, 
            std::uint64_t traderId, 
            Side side, 
            std::int64_t price, 
            std::uint64_t quantity, 
            int sequenceNumber
        );

        void reduceQuantity(std::uint64_t purchased);

        std::uint64_t getTraderId() const;

        Side getSide() const;

        std::int64_t getPrice() const;

        std::uint64_t getQuantity() const;

        std::uint64_t getOrderId() const;
};