#include <catch2/catch_test_macros.hpp>

#include "OrderBook.h"
#include "display/DebugDisplay.h"
#include "display/TraderDisplay.h"

#include <string>

TEST_CASE("Debug display shows latest trade and resting orders")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(202, Side::SELL, 5200, 8);

    // Partially fills the resting buy.
    book.addOrder(303, Side::SELL, 5000, 4);

    std::string display =
        formatDebugDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("LAST TRADE") != std::string::npos);
    REQUIRE(display.find("Trade: 1") != std::string::npos);
    REQUIRE(display.find("Price: $5000") != std::string::npos);

    REQUIRE(
        display.find("Order: 1 Trader: 101 Qty: 6")
        != std::string::npos
    );

    REQUIRE(
        display.find("Order: 2 Trader: 202 Qty: 8")
        != std::string::npos
    );
}

TEST_CASE("Debug display works when no trade has occurred")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(202, Side::SELL, 5200, 8);

    std::string display =
        formatDebugDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("LAST TRADE") == std::string::npos);
    REQUIRE(display.find("Order: 1 Trader: 101 Qty: 10") != std::string::npos);
    REQUIRE(display.find("Order: 2 Trader: 202 Qty: 8") != std::string::npos);
}

TEST_CASE("Trader display aggregates quantity at each price level")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(102, Side::BUY, 5000, 5);

    book.addOrder(201, Side::SELL, 5200, 8);
    book.addOrder(202, Side::SELL, 5200, 2);

    std::string display =
        formatTraderDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(
        display.find("Best Bid: $5000   | Qty: 15")
        != std::string::npos
    );

    REQUIRE(
        display.find("Best Ask: $5200   | Qty: 10")
        != std::string::npos
    );

    REQUIRE(display.find("Spread: $200") != std::string::npos);

    REQUIRE(display.find("5000     Qty: 15") != std::string::npos);
    REQUIRE(display.find("5200     Qty: 10") != std::string::npos);
}

TEST_CASE("Trader display handles one-sided book")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);

    std::string display =
        formatTraderDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(
        display.find("Best Bid: $5000   | Qty: 10")
        != std::string::npos
    );

    REQUIRE(display.find("Best Ask: None") != std::string::npos);
    REQUIRE(display.find("Spread: N/A") != std::string::npos);
}

TEST_CASE("Trader display handles empty book")
{
    OrderBook book;

    std::string display =
        formatTraderDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("Best Bid: None") != std::string::npos);
    REQUIRE(display.find("Best Ask: None") != std::string::npos);
    REQUIRE(display.find("Spread: N/A") != std::string::npos);
}

TEST_CASE("Trader display hides internal order information")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(202, Side::SELL, 5200, 8);

    std::string display =
        formatTraderDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("Order:") == std::string::npos);
    REQUIRE(display.find("Trader:") == std::string::npos);
}