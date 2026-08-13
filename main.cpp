#include <cstdint>
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <queue>
#include <list>
#include <unordered_map>
#include "OrderBook.h"

//Mock Unit Testing
int main() {
    OrderBook book;

    //correct order addition
    book.addOrder(10, 10000, Side::BUY, 50);
    if (book.containsOrder(1)) {
        std::cout << "Order successfully added.\n";
    }

    book.addOrder(20, 10000, Side::BUY, 30);
    
    //correct order cancellation on buy side
    book.cancelOrder(1, 10);

    if (!book.containsOrder(1)) {
        std::cout << "Order successfully cancelled.\n";
    }

    //correct order cancellation on sell side
    book.addOrder(22, 10000, Side::SELL, 30);
    if (book.containsOrder(3)) {
        std::cout << "Order successfully added.\n";
    }

    book.cancelOrder(3, 22);

    if (!book.containsOrder(3)) {
        std::cout << "Order successfully cancelled.\n";
    }

    //checks full correct removal of order
    try {
        book.cancelOrder(3, 22);
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << '\n';
    }

    //attempted cancellation by wrong trader
    book.addOrder(2, 10000, Side::SELL, 30);
    try {
        book.cancelOrder(4, 20);
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << '\n';
    }

    if (book.containsOrder(4)) {
        std::cout << "Order was correctly preserved.\n";
    }

    //catches nonexistent orders
    try {
        book.cancelOrder(9, 2);
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << '\n';
    }

    //constructor blocks low prices and quantities
    try {
        book.addOrder(1, 0, Side::BUY, 100);
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << '\n';
    }
    
    if (!book.containsOrder(5)) {
        std::cout << "Order was correctly not inserted.\n";
    }

    book.addOrder(2, 10000, Side::SELL, 30);
    if (book.containsOrder(5)) {
        std::cout << "Order was correctly inserted.\n";
    }

    try {
        book.addOrder(1, -1, Side::BUY, 10);
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << '\n';
    }

    if (!book.containsOrder(6)) {
        std::cout << "Order was correctly not inserted.\n";
    }

    try {
        book.addOrder(1, 10000, Side::BUY, 0);
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << '\n';
    }

    if (!book.containsOrder(7)) {
        std::cout << "Order was correctly not inserted.\n";
    }

    return 0;
}