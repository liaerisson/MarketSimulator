#include <iostream>

#include "OrderBook.h"
#include "display/DebugDisplay.h"

int main()
{
    OrderBook book;

    // Resting buy
    book.addOrder(101, Side::BUY, 5000, 10);

    // Resting sell
    book.addOrder(202, Side::SELL, 5200, 8);

    // This sell crosses the resting buy and creates one trade.
    book.addOrder(303, Side::SELL, 5000, 4);

    // Additional resting orders for display.
    book.addOrder(404, Side::BUY, 4900, 6);
    book.addOrder(505, Side::SELL, 5300, 12);

    BookSnapshot snapshot = book.getSnapshot();
    std::optional<Trade> lastTrade = book.getLastTrade();

    std::cout << formatDebugDisplay(snapshot, lastTrade);

    return 0;
}