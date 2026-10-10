#include "DebugDisplay.h"

#include <string>

std::string formatDebugDisplay(const BookSnapshot &snapshot, const std::optional<Trade> &lastTrade)
{
    std::string display = "BUY SIDE            SELL SIDE                    \n";

    std::vector<std::string> buyLines = formatPriceLevels(snapshot.buyPrices);
    std::vector<std::string> sellLines = formatPriceLevels(snapshot.sellPrices);

    int64_t longer = std::max(buyLines.size(), sellLines.size());

    int64_t padding = 20;

    for (std::size_t i = 0; i < longer; i++)
    {
        std::string line = "";

        if (i < buyLines.size())
        {
            line = buyLines[i];
        }

        std::size_t difference = padding - line.size();
        if (difference > 0)
        {
            for (int j = 0; j < difference; j++)
            {
                line.append(" ");
            }
        }

        if (i < sellLines.size())
        {
            line.append(sellLines[i]);
        }

        display.append(line);
        display.append("\n");
    }
}

std::vector<std::string> formatPriceLevels(const std::vector<PriceLevelSnapshot> &levels)
{

    std::vector<std::string> resultLines;

    for (const auto &level : levels)
    {
        std::string priceLine = std::to_string(level.price);
        resultLines.push_back(priceLine);

        for (const auto &order : level.orders)
        {
            std::string currentOrder = "Order: " + std::to_string(order.orderId) + " Trader: " + std::to_string(order.traderId) + " Quantity: " + std::to_string(order.quantity);
            resultLines.push_back(currentOrder);
        }
    }

    return resultLines;
}