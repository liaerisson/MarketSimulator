#include "OrderBook.h"
#include <stdexcept>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Order created correctly") {
    OrderBook book = OrderBook();
    book.addOrder(10, Side::BUY, 10000, 5);
    REQUIRE(book.containsOrder(1));
}

TEST_CASE("Orderbook rejects price of zero") {
    OrderBook book = OrderBook();
    REQUIRE_THROWS_AS(
        book.addOrder(10, Side::BUY, 0, 5), 
        std::invalid_argument
    );
    REQUIRE(!book.containsOrder(1));
}

TEST_CASE("Orderbook rejects negative price") {
    OrderBook book = OrderBook();
    REQUIRE_THROWS_AS(
        book.addOrder(10, Side::BUY, -500, 5), 
        std::invalid_argument
    );
    REQUIRE(!book.containsOrder(1));
}

TEST_CASE("Orderbook rejects zero quantity") {
    OrderBook book = OrderBook();
    REQUIRE_THROWS_AS(
        book.addOrder(10, Side::BUY, 10000, 0), 
        std::invalid_argument
    );
    REQUIRE_FALSE(book.containsOrder(1));
}

TEST_CASE("Orderbook correctly cancels order") {
    OrderBook book = OrderBook();
    book.addOrder(10, Side::BUY, 10000, 5);
    book.cancelOrder(1, 10);

    REQUIRE_FALSE(book.containsOrder(1));
}

TEST_CASE("Orderbook rejects cancelling by wrong trader") {
    OrderBook book = OrderBook();
    book.addOrder(10, Side::BUY, 10000, 5);
    REQUIRE_THROWS_AS(
        book.cancelOrder(1, 5),
        std::invalid_argument
    );
    REQUIRE(book.containsOrder(1));
}

TEST_CASE("Orderbook rejects cancelling nonexistent order") {
    OrderBook book = OrderBook();
    book.addOrder(10, Side::BUY, 10000, 5);
    REQUIRE_THROWS_AS(
        book.cancelOrder(2, 10),
        std::invalid_argument
    );
    REQUIRE(book.containsOrder(1));
    REQUIRE_FALSE(book.containsOrder(2));
}

TEST_CASE("containsOrder tracks active orders") {
    OrderBook book;
    REQUIRE_FALSE(book.containsOrder(1));

    book.addOrder(10, Side::BUY, 5000, 10);
    REQUIRE(book.containsOrder(1));

    book.cancelOrder(1, 10);
    REQUIRE_FALSE(book.containsOrder(1));
}