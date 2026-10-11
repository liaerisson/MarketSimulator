#include <catch2/catch_test_macros.hpp>

#include "OrderBook.h"
#include "display/DebugDisplay.h"
#include "display/TraderDisplay.h"

#include <string>

TEST_CASE("Debug display handles empty book with no trades")
{
    OrderBook book;

    std::string display =
        formatDebugDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("BUY SIDE") != std::string::npos);
    REQUIRE(display.find("SELL SIDE") != std::string::npos);
    REQUIRE(display.find("LAST TRADE") == std::string::npos);
    REQUIRE(display.find("Order:") == std::string::npos);
}

TEST_CASE("Debug display handles buy-only book")
{
    OrderBook book;

    auto id = book.addOrder(101, Side::BUY, 5000, 10);

    std::string display =
        formatDebugDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("5000") != std::string::npos);

    REQUIRE(
        display.find(
            "Order: " + std::to_string(id) +
            " Trader: 101 Qty: 10"
        ) != std::string::npos
    );

    REQUIRE(display.find("LAST TRADE") == std::string::npos);
}

TEST_CASE("Debug display handles sell-only book")
{
    OrderBook book;

    auto id = book.addOrder(201, Side::SELL, 5200, 8);

    std::string display =
        formatDebugDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("5200") != std::string::npos);

    REQUIRE(
        display.find(
            "Order: " + std::to_string(id) +
            " Trader: 201 Qty: 8"
        ) != std::string::npos
    );
}

TEST_CASE("Debug display shows remaining quantity after partial fill")
{
    OrderBook book;

    auto buyId = book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(202, Side::SELL, 5000, 4);

    std::string display =
        formatDebugDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(
        display.find(
            "Order: " + std::to_string(buyId) +
            " Trader: 101 Qty: 6"
        ) != std::string::npos
    );

    REQUIRE(display.find("LAST TRADE") != std::string::npos);
    REQUIRE(display.find("Price: $5000") != std::string::npos);
    REQUIRE(display.find("Qty: 4") != std::string::npos);
}

TEST_CASE("Debug display shows multiple orders at same price")
{
    OrderBook book;

    auto firstId = book.addOrder(101, Side::BUY, 5000, 10);
    auto secondId = book.addOrder(102, Side::BUY, 5000, 20);

    std::string display =
        formatDebugDisplay(book.getSnapshot(), book.getLastTrade());

    std::string firstOrder =
        "Order: " + std::to_string(firstId) +
        " Trader: 101 Qty: 10";

    std::string secondOrder =
        "Order: " + std::to_string(secondId) +
        " Trader: 102 Qty: 20";

    auto firstPosition = display.find(firstOrder);
    auto secondPosition = display.find(secondOrder);

    REQUIRE(firstPosition != std::string::npos);
    REQUIRE(secondPosition != std::string::npos);
    REQUIRE(firstPosition < secondPosition);
}

TEST_CASE("Debug display shows only most recent trade")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(201, Side::SELL, 5000, 10);

    book.addOrder(102, Side::BUY, 5100, 5);
    book.addOrder(202, Side::SELL, 5100, 5);

    std::string display =
        formatDebugDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("Trade: 2") != std::string::npos);
    REQUIRE(display.find("Price: $5100") != std::string::npos);
}

TEST_CASE("Debug display retains last trade when current book is empty")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(201, Side::SELL, 5000, 10);

    std::string display =
        formatDebugDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("LAST TRADE") != std::string::npos);
    REQUIRE(display.find("Price: $5000") != std::string::npos);
    REQUIRE(display.find("Order:") == std::string::npos);
}

TEST_CASE("Debug display shows resting-order execution price for crossed trade")
{
    OrderBook book;

    book.addOrder(101, Side::SELL, 5000, 10);
    book.addOrder(201, Side::BUY, 5200, 4);

    std::string display =
        formatDebugDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("Price: $5000") != std::string::npos);
    REQUIRE(display.find("Qty: 4") != std::string::npos);
}

TEST_CASE("Trader display handles empty book")
{
    OrderBook book;

    std::string display =
        formatTraderDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("Best Bid: None") != std::string::npos);
    REQUIRE(display.find("Best Ask: None") != std::string::npos);
    REQUIRE(display.find("Spread: N/A") != std::string::npos);
    REQUIRE(display.find("LAST TRADE") == std::string::npos);
}

TEST_CASE("Trader display handles buy-only book")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(102, Side::BUY, 5000, 5);

    std::string display =
        formatTraderDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(
        display.find("Best Bid: $5000   | Qty: 15")
        != std::string::npos
    );

    REQUIRE(display.find("Best Ask: None") != std::string::npos);
    REQUIRE(display.find("Spread: N/A") != std::string::npos);
}

TEST_CASE("Trader display handles sell-only book")
{
    OrderBook book;

    book.addOrder(201, Side::SELL, 5200, 8);
    book.addOrder(202, Side::SELL, 5200, 2);

    std::string display =
        formatTraderDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("Best Bid: None") != std::string::npos);

    REQUIRE(
        display.find("Best Ask: $5200   | Qty: 10")
        != std::string::npos
    );

    REQUIRE(display.find("Spread: N/A") != std::string::npos);
}

TEST_CASE("Trader display shows best bid best ask and spread")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(201, Side::SELL, 5200, 8);

    std::string display =
        formatTraderDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(
        display.find("Best Bid: $5000   | Qty: 10")
        != std::string::npos
    );

    REQUIRE(
        display.find("Best Ask: $5200   | Qty: 8")
        != std::string::npos
    );

    REQUIRE(display.find("Spread: $200") != std::string::npos);
}

TEST_CASE("Trader display aggregates quantity at best price levels")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(102, Side::BUY, 5000, 5);
    book.addOrder(103, Side::BUY, 4900, 7);

    book.addOrder(201, Side::SELL, 5200, 8);
    book.addOrder(202, Side::SELL, 5200, 2);
    book.addOrder(203, Side::SELL, 5300, 9);

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

    REQUIRE(display.find("5000     Qty: 15") != std::string::npos);
    REQUIRE(display.find("4900     Qty: 7") != std::string::npos);

    REQUIRE(display.find("5200     Qty: 10") != std::string::npos);
    REQUIRE(display.find("5300     Qty: 9") != std::string::npos);
}

TEST_CASE("Trader display preserves price-level ordering")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 4900, 1);
    book.addOrder(102, Side::BUY, 5100, 1);
    book.addOrder(103, Side::BUY, 5000, 1);

    book.addOrder(201, Side::SELL, 5400, 1);
    book.addOrder(202, Side::SELL, 5200, 1);
    book.addOrder(203, Side::SELL, 5300, 1);

    std::string display =
        formatTraderDisplay(book.getSnapshot(), book.getLastTrade());

    auto buy5100 = display.find("5100     Qty: 1");
    auto buy5000 = display.find("5000     Qty: 1");
    auto buy4900 = display.find("4900     Qty: 1");

    REQUIRE(buy5100 < buy5000);
    REQUIRE(buy5000 < buy4900);

    auto sell5200 = display.find("5200     Qty: 1");
    auto sell5300 = display.find("5300     Qty: 1");
    auto sell5400 = display.find("5400     Qty: 1");

    REQUIRE(sell5200 < sell5300);
    REQUIRE(sell5300 < sell5400);
}

TEST_CASE("Trader display shows latest trade using market-facing fields")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(201, Side::SELL, 5000, 4);

    std::string display =
        formatTraderDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("LAST TRADE") != std::string::npos);
    REQUIRE(display.find("Price: $5000") != std::string::npos);
    REQUIRE(display.find("Qty: 4") != std::string::npos);

    REQUIRE(display.find("Trade:") == std::string::npos);
    REQUIRE(display.find("Order:") == std::string::npos);
    REQUIRE(display.find("Trader:") == std::string::npos);
}

TEST_CASE("Trader display uses resting price for crossed trade")
{
    OrderBook book;

    book.addOrder(101, Side::SELL, 5000, 10);
    book.addOrder(201, Side::BUY, 5200, 4);

    std::string display =
        formatTraderDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("Price: $5000") != std::string::npos);
    REQUIRE(display.find("Qty: 4") != std::string::npos);
}

TEST_CASE("Trader display retains last trade when current book is empty")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(201, Side::SELL, 5000, 10);

    std::string display =
        formatTraderDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(display.find("LAST TRADE") != std::string::npos);
    REQUIRE(display.find("Price: $5000") != std::string::npos);
    REQUIRE(display.find("Qty: 10") != std::string::npos);

    REQUIRE(display.find("Best Bid: None") != std::string::npos);
    REQUIRE(display.find("Best Ask: None") != std::string::npos);
    REQUIRE(display.find("Spread: N/A") != std::string::npos);
}

TEST_CASE("Trader display reflects one remaining side after trade")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(201, Side::SELL, 5000, 4);

    std::string display =
        formatTraderDisplay(book.getSnapshot(), book.getLastTrade());

    REQUIRE(
        display.find("Best Bid: $5000   | Qty: 6")
        != std::string::npos
    );

    REQUIRE(display.find("Best Ask: None") != std::string::npos);
    REQUIRE(display.find("Spread: N/A") != std::string::npos);

    REQUIRE(display.find("Price: $5000") != std::string::npos);
    REQUIRE(display.find("Qty: 4") != std::string::npos);
}