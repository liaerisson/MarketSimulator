#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>
#include <stdexcept>

enum class Side {
    BUY, 
    SELL
};

class Order {
    private:
        uint64_t orderId;
        uint64_t traderId;
        int64_t price;
        Side side;
        //unsigned int with validation
        uint64_t quantity;
        int sequenceNumber;

    public:
        Order(uint64_t orderId, uint64_t traderId, Side side, int64_t price, uint64_t quantity, int sequenceNumber) {
            if(price <= 0) {
                throw std::invalid_argument("Price must be positive.");
            } else if(quantity == 0) {
                throw std::invalid_argument("Quantity must be >0.");
            }

            this->orderId = orderId;
            this->traderId = traderId;
            this->price = price;
            this->side = side;
            this->quantity = quantity;
            this->sequenceNumber = sequenceNumber;
        }

        void reduceQuantity(std::uint64_t purchased) {
            if(purchased == 0 || purchased > quantity) {
                throw std::invalid_argument("Invalid amount to fill.");
            }

            this->quantity = quantity - purchased;
        }
};