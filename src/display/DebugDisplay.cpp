#include "DebugDisplay.h"

#include <string>

std::string formatDebugDisplay(const BookSnapshot& snapshot, const std::optional<Trade>& lastTrade) {
    std::string header = "BUY SIDE                              SELL SIDE";
    std::list<std::string> buyLines;
    std::list<std::string> sellLines;

    
}

std::vector<std::string> formatPriceLevels(const std::vector<PriceLevelSnapshot>& levels) {
    
    std::vector<std::string> resultLines;

    for (const auto& level : levels) {
        std::string priceLine = std::to_string(level.price);
        resultLines.push_back(priceLine);

        for(const auto& order : level.orders) {
            std::string currentOrder = "Order: " + std::to_string(order.orderId) + " Trader: " + std::to_string(order.traderId) + " Quantity: " + std::to_string(order.quantity);
            resultLines.push_back(currentOrder);
        }
    }

    return resultLines;
}