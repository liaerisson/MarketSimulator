#include "display/DebugDisplay.h"

#include <string>

std::vector<std::string> formatPriceLevels(const std::vector<PriceLevelSnapshot> &levels)
{

    std::vector<std::string> resultLines;

    for (const auto &level : levels)
    {
        std::string priceLine = std::to_string(level.price);
        resultLines.push_back(priceLine);

        for (const auto &order : level.orders)
        {
            std::string currentOrder = "Order: " + std::to_string(order.orderId) + " Trader: " + std::to_string(order.traderId) + " Qty: " + std::to_string(order.quantity);
            resultLines.push_back(currentOrder);
        }
    }

    return resultLines;
}

std::string formatDebugDisplay(const BookSnapshot &snapshot, const std::optional<Trade> &lastTrade)
{
    std::string tradeDisplay;
    if (lastTrade.has_value())
    {
        tradeDisplay.append("LAST TRADE\n________________________________________________________________________\n");
        tradeDisplay.append("Trade: " + std::to_string(lastTrade->tradeId) + "   | Buy: " + std::to_string(lastTrade->buyOrderId) + "   | Sell: " + std::to_string(lastTrade->sellOrderId) + "   | Price: $" + std::to_string(lastTrade->price) + "   | Qty: " + std::to_string(lastTrade->quantity) + "\n");
    }

    std::string orderDisplay = "________________________________________________________________________\n";
    orderDisplay.append("BUY SIDE                                SELL SIDE                              \n");

    std::vector<std::string> buyLines = formatPriceLevels(snapshot.buyPrices);
    std::vector<std::string> sellLines = formatPriceLevels(snapshot.sellPrices);

    std::size_t longer = std::max(buyLines.size(), sellLines.size());

    std::size_t padding = 40;

    for (std::size_t i = 0; i < longer; i++)
    {
        std::string line = "";

        if (i < buyLines.size())
        {
            line = buyLines[i];
        }

        if (line.size() < padding)
        {
            std::size_t difference = padding - line.size();
            for (std::size_t j = 0; j < difference; j++)
            {
                line.append(" ");
            }
        }

        if (i < sellLines.size())
        {
            line.append(sellLines[i]);
        }

        orderDisplay.append(line);
        orderDisplay.append("\n");
    }

    std::string display = tradeDisplay;
    display.append(orderDisplay);
    display.append("\n");
    return display;
}