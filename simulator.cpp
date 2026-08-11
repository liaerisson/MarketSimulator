#include <cstdint>
#include <stdexcept>

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

        static std::int64_t validatePrice(std::int64_t price) {
            if(price <= 0) {
                throw std::invalid_argument("Price must be positive.");
            }
            return price;
        }

        static std::uint64_t validateQuantity(std::uint64_t quantity) {
            if(quantity == 0) {
                throw std::invalid_argument("Quantity must be >0.");
            }
            return quantity;
        }

    public:
        Order(std::uint64_t orderId, std::uint64_t traderId, 
            Side side, std::int64_t price, 
            std::uint64_t quantity, int sequenceNumber) 
            : orderId(orderId), traderId(traderId), side(side), sequenceNumber(sequenceNumber), price(validatePrice(price)), quantity(validateQuantity(quantity))
            {
            
        }

        void reduceQuantity(std::uint64_t purchased) {
            if(purchased == 0 || purchased > quantity) {
                throw std::invalid_argument("Invalid amount to fill.");
            }

            this->quantity = quantity - purchased;
        }
};