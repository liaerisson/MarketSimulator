#include <iostream>

#include "OrderBook.h"
#include "display/DebugDisplay.h"
#include "display/TraderDisplay.h"

int main()
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(202, Side::SELL, 5200, 8);
    book.addOrder(303, Side::SELL, 5000, 4);
    book.addOrder(404, Side::BUY, 4900, 6);
    book.addOrder(505, Side::SELL, 5300, 12);

    BookSnapshot snapshot = book.getSnapshot();
    std::optional<Trade> lastTrade = book.getLastTrade();

    std::cout << "DEBUG VIEW\n";
    std::cout << formatDebugDisplay(snapshot, lastTrade);

    std::cout << "\nTRADER VIEW\n";
    std::cout << formatTraderDisplay(snapshot, lastTrade);

    return 0;
}