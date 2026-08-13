#include <cstdint>
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <queue>
#include <list>
#include <unordered_map>
#include "OrderBook.h"

int main() {
    OrderBook book;

    book.addOrder(10, 10000, Side::BUY, 50);
    book.addOrder(20, 10000, Side::BUY, 30);

    book.cancelOrder(1, 10);

    if (!book.containsOrder(1)) {
        std::cout << "Order successfully cancelled.\n";
    }

    return 0;
}