#include "display/TraderDisplay.h"

#include <string>

std::vector<std::string> formatTraderPriceLevels(const std::vector<PriceLevelSnapshot> &levels)
{

    std::vector<std::string> resultLines;

    for (const auto &level : levels)
    {
        std::string priceLine = std::to_string(level.price);

        std::uint64_t total = 0;
        for (const auto &order : level.orders)
        {
            total += order.quantity;
        }
        std::string currentLevel = priceLine + "     Qty: " + std::to_string(total);
        resultLines.push_back(currentLevel);
    }

    return resultLines;
}

std::string formatTraderDisplay(const BookSnapshot &snapshot, const std::optional<Trade> &lastTrade)
{
    std::string tradeDisplay;
    if (lastTrade.has_value())
    {
        tradeDisplay.append("LAST TRADE\n________________________________________________________________________\n");
        tradeDisplay.append("Price: $" + std::to_string(lastTrade->price) + "   | Qty: " + std::to_string(lastTrade->quantity) + "\n");
    }

    std::string marketDisplay = "MARKET INFORMATION\n________________________________________________________________________\n";
    
    bool hasBids = !snapshot.buyPrices.empty();
    bool hasAsks = !snapshot.sellPrices.empty();

    if (hasBids) {
        std::uint64_t bestBidQuantity = 0;
        for (const auto &order : snapshot.buyPrices.front().orders) {
            bestBidQuantity += order.quantity;
        }

        marketDisplay.append(
            "Best Bid: $" + std::to_string(snapshot.buyPrices.front().price) + "   | Qty: " + std::to_string(bestBidQuantity) + "\n");
    } else {
        marketDisplay.append("Best Bid: None\n");
    }

    if (hasAsks) {
        std::uint64_t bestAskQuantity = 0;

        for (const auto &order : snapshot.sellPrices.front().orders) {
            bestAskQuantity += order.quantity;
        }

        marketDisplay.append("Best Ask: $" + std::to_string(snapshot.sellPrices.front().price) + "   | Qty: " + std::to_string(bestAskQuantity) + "\n");
    } else {
        marketDisplay.append("Best Ask: None\n");
    }

    if (hasBids && hasAsks) {
        std::int64_t spread = snapshot.sellPrices.front().price - snapshot.buyPrices.front().price;
        marketDisplay.append("Spread: $" + std::to_string(spread) + "\n");
    } else {
        marketDisplay.append("Spread: N/A\n");
    }

    std::string orderDisplay = "________________________________________________________________________\n";
    orderDisplay.append("BUY SIDE                                SELL SIDE                              \n");

    std::vector<std::string> buyLines = formatTraderPriceLevels(snapshot.buyPrices);
    std::vector<std::string> sellLines = formatTraderPriceLevels(snapshot.sellPrices);

    std::size_t longer = std::max(buyLines.size(), sellLines.size());
    std::size_t padding = 40;

    for (std::size_t i = 0; i < longer; i++) {
        std::string line = "";

        if (i < buyLines.size()) {
            line = buyLines[i];
        }

        if (line.size() < padding) {
            std::size_t difference = padding - line.size();
            for (std::size_t j = 0; j < difference; j++) {
                line.append(" ");
            }
        }

        if (i < sellLines.size()) {
            line.append(sellLines[i]);
        }

        orderDisplay.append(line);
        orderDisplay.append("\n");
    }

    std::string display = tradeDisplay;
    
    display.append(marketDisplay);
    display.append(orderDisplay);
    display.append("\n");
    return display;
}