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

        std::uint64_t getTraderId() const {
            return traderId;
        }
};

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

        std::uint64_t getNewOrderId() {
            return orderId++;
        }

        int getNewSequenceNumber() {
            return sequenceNumber++;
        }


    public:
        OrderBook() : orderId(1), sequenceNumber(1) 
        {}

        void addOrder(std::uint64_t traderId, std::int64_t price, Side side, std::uint64_t quantity) {
            std::uint64_t order = getNewOrderId();
            int sequenceNum = getNewSequenceNumber();
            Order newOrder = Order(order, traderId, side, price, quantity, sequenceNum);
            
            OrderLocation location;
            location.price = price;
            location.side = side;

            if(side == Side::BUY) {
                //group references the actual list in the map, so manipulations still apply to it
                auto& group = buys[price];
                group.push_back(newOrder);
                location.iterator = std::prev(group.end());
            
            } else if(side == Side::SELL) {
                auto& group = sells[price];
                group.push_back(newOrder);
                location.iterator = std::prev(group.end());
            }

            orderLookup.insert({order, location});
        }

        void cancelOrder(std::uint64_t orderId, std::uint64_t traderId) {
            Order currentOrder = *(orderLookup.at(orderId).iterator);
            if(currentOrder.getTraderId() != traderId) {
                throw std::invalid_argument("Only the trader who made the order may cancel it.");
            }
        }
};