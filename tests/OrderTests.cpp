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