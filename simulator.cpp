#include <cstdint>
#include <stdexcept>
#include <map>
#include <queue>
#include <list>
#include <unordered_map>

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
            : orderId(orderId), traderId(traderId), price(validatePrice(price)), side(side), quantity(validateQuantity(quantity)), sequenceNumber(sequenceNumber)
            {
            
        }

        void reduceQuantity(std::uint64_t purchased) {
            if(purchased == 0 || purchased > quantity) {
                throw std::invalid_argument("Invalid amount to fill.");
            }

            this->quantity = quantity - purchased;
        }
};

struct OrderLocation {
            Side side;
            std::int64_t price;
            std::list<Order>::iterator iterator;
};

class OrderBook {
    private:
        std::map<std::int64_t, std::list<Order>> sells;
        std::map<std::int64_t, std::list<Order>, std::greater<std::int64_t>> buys;
        std::unordered_map<std::uint64_t, OrderLocation> orderLookup;
        uint64_t orderId;
        int sequenceNumber;

        std::uint64_t getNewOrderId() {
            return orderId++;
        }

        int getNewSequenceNumber() {
            return sequenceNumber++;
        }


    public:
        OrderBook() : orderId(1), sequenceNumber(1) 
        {
        }

        void addOrder(std::uint64_t traderId, std::int64_t price, Side side, uint64_t quantity) {
            Order newOrder = Order(getNewOrderId(), traderId, side, price, quantity, getNewSequenceNumber());
            if(side == Side::BUY) {
                buys[price].push_back({newOrder});
            }
        }
};