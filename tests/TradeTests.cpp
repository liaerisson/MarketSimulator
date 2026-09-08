#include "Trade.h"
#include <stdexcept>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Trade stores values correctly") {
    Trade trade{
        1,
        10,
        20,
        100,
        200,
        5000,
        25
    };

    REQUIRE(trade.tradeId == 1);
    REQUIRE(trade.buyOrderId == 10);
    REQUIRE(trade.sellOrderId == 20);
    REQUIRE(trade.buyerTraderId == 100);
    REQUIRE(trade.sellerTraderId == 200);
    REQUIRE(trade.price == 5000);
    REQUIRE(trade.quantity == 25);
}