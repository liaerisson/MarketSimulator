#include "Order.h"
#include <stdexcept>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Order rejects zero quantity") {
    REQUIRE_THROWS_AS(
        Order(1, 10, Side::BUY, 10000, 0, 1),
        std::invalid_argument
    );
}

TEST_CASE("Order rejects negative price") {
    REQUIRE_THROWS_AS(
        Order(1, 10, Side::BUY, -500, 10, 1),
        std::invalid_argument
    );
}

TEST_CASE("Order not thrown if valid") {
    REQUIRE_NOTHROW(
        Order(1, 10, Side::BUY, 10000, 10, 1)
    );
}

TEST_CASE("Order values stored correctly") {
    Order newOrder = Order(1, 10, Side::BUY, 10000, 10, 1);
    REQUIRE(newOrder.getOrderId() == 1);
    REQUIRE(newOrder.getTraderId() == 10);
    REQUIRE(newOrder.getSide() == Side::BUY);
    REQUIRE(newOrder.getPrice() == 10000); 
    REQUIRE(newOrder.getQuantity() == 10);
}

TEST_CASE("Order rejects reduction of 0") {
    Order newOrder = Order(1, 10, Side::BUY, 10000, 10, 1);
    REQUIRE_THROWS_AS(
        newOrder.reduceQuantity(0), std::invalid_argument
    );
}

TEST_CASE("Order rejects reduction greater than quantity") {
    Order newOrder = Order(1, 10, Side::BUY, 10000, 10, 1);
    REQUIRE_THROWS_AS(
        newOrder.reduceQuantity(11), std::invalid_argument
    );
}

TEST_CASE("Order reduces from quantity correctly") {
    Order newOrder = Order(1, 10, Side::BUY, 10000, 10, 1);
    newOrder.reduceQuantity(5);
    REQUIRE(newOrder.getQuantity() == 5);
}

TEST_CASE("Order reduces full quantity correctly") {
    Order newOrder = Order(1, 10, Side::BUY, 10000, 10, 1);
    newOrder.reduceQuantity(10);
    REQUIRE(newOrder.getQuantity() == 0);
}