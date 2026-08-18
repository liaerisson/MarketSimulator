#include "OrderBook.h"
#include <stdexcept>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Order created correctly") {
    OrderBook book;
    auto id = book.addOrder(10, Side::BUY, 10000, 5);
    REQUIRE(book.containsOrder(id));
}

TEST_CASE("Orderbook rejects price of zero") {
    OrderBook book;
    REQUIRE_THROWS_AS(
        book.addOrder(10, Side::BUY, 0, 5), 
        std::invalid_argument
    );
    REQUIRE_FALSE(book.containsOrder(1));
}

TEST_CASE("Orderbook rejects negative price") {
    OrderBook book;
    REQUIRE_THROWS_AS(
        book.addOrder(10, Side::BUY, -500, 5), 
        std::invalid_argument
    );
    REQUIRE_FALSE(book.containsOrder(1));
}

TEST_CASE("Orderbook rejects zero quantity") {
    OrderBook book;
    REQUIRE_THROWS_AS(
        book.addOrder(10, Side::BUY, 10000, 0), 
        std::invalid_argument
    );
    REQUIRE_FALSE(book.containsOrder(1));
}

TEST_CASE("Orderbook correctly cancels order") {
    OrderBook book;
    book.addOrder(10, Side::BUY, 10000, 5);
    book.cancelOrder(1, 10);

    REQUIRE_FALSE(book.containsOrder(1));
}

TEST_CASE("Orderbook rejects cancelling by wrong trader") {
    OrderBook book;
    book.addOrder(10, Side::BUY, 10000, 5);
    REQUIRE_THROWS_AS(
        book.cancelOrder(1, 5),
        std::invalid_argument
    );
    REQUIRE(book.containsOrder(1));
}

TEST_CASE("Orderbook rejects cancelling nonexistent order") {
    OrderBook book;
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

TEST_CASE("orderId begins at one") {
    OrderBook book;
    REQUIRE_FALSE(book.containsOrder(1));

    auto id = book.addOrder(10, Side::BUY, 5000, 10);
    REQUIRE(id == 1);
    REQUIRE(book.containsOrder(1));

    book.cancelOrder(1, 10);
    REQUIRE_FALSE(book.containsOrder(1));
}

TEST_CASE("Orderbook sorts buys in descending order") {
    OrderBook book;
    book.addOrder(1, Side::BUY, 5000, 10);
    REQUIRE(book.getBestBid() == 5000);

    book.addOrder(2, Side::BUY, 15000, 10);
    REQUIRE(book.getBestBid() == 15000);
}

TEST_CASE("Orderbook sorts sells in ascending order") {
    OrderBook book;
    book.addOrder(1, Side::SELL, 15000, 10);
    REQUIRE(book.getBestAsk() == 15000);

    book.addOrder(2, Side::SELL, 5000, 10);
    REQUIRE(book.getBestAsk() == 5000);
}

TEST_CASE("Orderbook removes price category correctly") {
    OrderBook book;
    auto id = book.addOrder(1, Side::BUY, 5000, 10);
    REQUIRE(book.getBestBid() == 5000);

    book.cancelOrder(id, 1);
    REQUIRE_FALSE(book.getBestBid().has_value());
}

TEST_CASE("Orderbook keeps non-crossing orders") {
    OrderBook book;
    auto id = book.addOrder(1, Side::BUY, 5000, 10);
    auto idTwo = book.addOrder(2, Side::SELL, 6000, 5);

    REQUIRE(book.getOrderQuantity(id) == 10);
    REQUIRE(book.getOrderQuantity(idTwo) == 5);
}

TEST_CASE("Orderbook matches at price boundary but not below it") {    OrderBook book;
    auto id = book.addOrder(1, Side::SELL, 5000, 10);
    auto idTwo = book.addOrder(2, Side::BUY, 4999, 5);
    
    REQUIRE(book.getOrderQuantity(idTwo) == 5);
    REQUIRE(book.getOrderQuantity(id) == 10);

    auto idThree = book.addOrder(3, Side::BUY, 5000, 10);
    REQUIRE(book.getOrderQuantity(idTwo) == 5);
    REQUIRE_FALSE(book.containsOrder(id));
    REQUIRE_FALSE(book.containsOrder(idThree));
}

TEST_CASE("Orderbook matches at sell price boundary but not above it") {
    OrderBook book;

    auto buyId = book.addOrder(1, Side::BUY, 5000, 10);
    auto sellId = book.addOrder(2, Side::SELL, 5001, 5);

    REQUIRE(book.getOrderQuantity(buyId) == 10);
    REQUIRE(book.getOrderQuantity(sellId) == 5);

    auto sellIdTwo = book.addOrder(3, Side::SELL, 5000, 10);

    REQUIRE(book.getOrderQuantity(sellId) == 5);
    REQUIRE_FALSE(book.containsOrder(buyId));
    REQUIRE_FALSE(book.containsOrder(sellIdTwo));
}

TEST_CASE("Orderbook fully fills two orders with equal quantities") {
    OrderBook book;
    auto id = book.addOrder(1, Side::BUY, 5000, 10);
    auto idTwo = book.addOrder(2, Side::SELL, 5000, 10);

    REQUIRE_FALSE(book.containsOrder(id));
    REQUIRE_FALSE(book.containsOrder(idTwo));
}

TEST_CASE("Orderbook completes correct partial fill of resting order") {
    OrderBook book;
    auto id = book.addOrder(1, Side::BUY, 5000, 10);
    auto idTwo = book.addOrder(3, Side::SELL, 5000, 5);

    REQUIRE_FALSE(book.containsOrder(idTwo));
    REQUIRE(book.getOrderQuantity(id) == 5);
}

TEST_CASE("Orderbook completes correct partial fill of incoming order") {
    OrderBook book;
    auto id = book.addOrder(1, Side::BUY, 5000, 5);
    auto idTwo = book.addOrder(3, Side::SELL, 5000, 10);

    REQUIRE_FALSE(book.containsOrder(id));
    REQUIRE(book.getOrderQuantity(idTwo) == 5);
}

TEST_CASE("Orderbook handles one incoming order causing multiple fills") {
    OrderBook book;
    auto id = book.addOrder(1, Side::BUY, 5000, 10);
    auto id2 = book.addOrder(2, Side::BUY, 5000, 100);
    auto id3 = book.addOrder(3, Side::BUY, 5000, 150);
    auto id4 = book.addOrder(4, Side::SELL, 4000, 255);

    REQUIRE_FALSE(book.containsOrder(id4));
    REQUIRE_FALSE(book.containsOrder(id));
    REQUIRE_FALSE(book.containsOrder(id2));
    REQUIRE(book.getOrderQuantity(id3) == 5);
}

TEST_CASE("Orderbook follows FIFO structure for incoming sells") {
    OrderBook book;
    auto id = book.addOrder(1, Side::BUY, 5000, 10);
    auto id2 = book.addOrder(2, Side::BUY, 5000, 100);
    auto id3 = book.addOrder(3, Side::BUY, 5000, 150);
    
    auto id4 = book.addOrder(4, Side::SELL, 4000, 50);
    REQUIRE_FALSE(book.containsOrder(id4));
    REQUIRE_FALSE(book.containsOrder(id));
    REQUIRE(book.getOrderQuantity(id2) == 60);
    REQUIRE(book.getOrderQuantity(id3) == 150);

    auto id5 = book.addOrder(5, Side::SELL, 4000, 100);
    REQUIRE_FALSE(book.containsOrder(id5));
    REQUIRE_FALSE(book.containsOrder(id2));
    REQUIRE(book.getOrderQuantity(id3) == 110);

    auto id6 = book.addOrder(6, Side::SELL, 4000, 50);
    REQUIRE_FALSE(book.containsOrder(id6));
    REQUIRE(book.getOrderQuantity(id3) == 60);
}

TEST_CASE("Orderbook follows FIFO structure for incoming buys") {
    OrderBook book;
    auto id = book.addOrder(1, Side::SELL, 5000, 10);
    auto id2 = book.addOrder(2, Side::SELL, 5000, 100);
    auto id3 = book.addOrder(3, Side::SELL, 5000, 150);
    
    auto id4 = book.addOrder(4, Side::BUY, 6000, 50);
    REQUIRE_FALSE(book.containsOrder(id4));
    REQUIRE_FALSE(book.containsOrder(id));
    REQUIRE(book.getOrderQuantity(id2) == 60);
    REQUIRE(book.getOrderQuantity(id3) == 150);

    auto id5 = book.addOrder(5, Side::BUY, 6000, 100);
    REQUIRE_FALSE(book.containsOrder(id5));
    REQUIRE_FALSE(book.containsOrder(id2));
    REQUIRE(book.getOrderQuantity(id3) == 110);

    auto id6 = book.addOrder(6, Side::BUY, 6000, 50);
    REQUIRE_FALSE(book.containsOrder(id6));
    REQUIRE(book.getOrderQuantity(id3) == 60);
}

TEST_CASE("Incoming sell matches highest bid first") {
    OrderBook book;
    auto buyId = book.addOrder(1, Side::BUY, 5000, 10);
    auto buyId2 = book.addOrder(2, Side::BUY, 7000, 10);
    
    auto sellId = book.addOrder(3, Side::SELL, 4000, 10);
    REQUIRE_FALSE(book.containsOrder(buyId2));
    REQUIRE(book.getOrderQuantity(buyId) == 10);
    REQUIRE_FALSE(book.containsOrder(sellId));
}

TEST_CASE("Incoming buy matches lowest ask first") {
    OrderBook book;
    auto sellId = book.addOrder(1, Side::SELL, 5000, 10);
    auto sellId2 = book.addOrder(2, Side::SELL, 7000, 10);
    
    auto buyId = book.addOrder(3, Side::BUY, 8000, 10);
    REQUIRE_FALSE(book.containsOrder(sellId));
    REQUIRE(book.getOrderQuantity(sellId2) == 10);
    REQUIRE_FALSE(book.containsOrder(buyId));
}