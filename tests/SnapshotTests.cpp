#include <catch2/catch_test_macros.hpp>

#include "OrderBook.h"

TEST_CASE("Empty order book produces empty snapshot")
{
    OrderBook book;

    BookSnapshot snapshot = book.getSnapshot();

    REQUIRE(snapshot.buyPrices.empty());
    REQUIRE(snapshot.sellPrices.empty());
}

TEST_CASE("Snapshot preserves buy price ordering")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 4900, 10);
    book.addOrder(102, Side::BUY, 5100, 10);
    book.addOrder(103, Side::BUY, 5000, 10);

    BookSnapshot snapshot = book.getSnapshot();

    REQUIRE(snapshot.buyPrices.size() == 3);
    REQUIRE(snapshot.sellPrices.empty());

    REQUIRE(snapshot.buyPrices[0].price == 5100);
    REQUIRE(snapshot.buyPrices[1].price == 5000);
    REQUIRE(snapshot.buyPrices[2].price == 4900);
}

TEST_CASE("Snapshot preserves sell price ordering")
{
    OrderBook book;

    book.addOrder(201, Side::SELL, 5400, 10);
    book.addOrder(202, Side::SELL, 5200, 10);
    book.addOrder(203, Side::SELL, 5300, 10);

    BookSnapshot snapshot = book.getSnapshot();

    REQUIRE(snapshot.sellPrices.size() == 3);
    REQUIRE(snapshot.buyPrices.empty());

    REQUIRE(snapshot.sellPrices[0].price == 5200);
    REQUIRE(snapshot.sellPrices[1].price == 5300);
    REQUIRE(snapshot.sellPrices[2].price == 5400);
}

TEST_CASE("Snapshot preserves FIFO order within a price level")
{
    OrderBook book;

    auto firstId = book.addOrder(101, Side::BUY, 5000, 10);
    auto secondId = book.addOrder(102, Side::BUY, 5000, 20);
    auto thirdId = book.addOrder(103, Side::BUY, 5000, 30);

    BookSnapshot snapshot = book.getSnapshot();

    REQUIRE(snapshot.buyPrices.size() == 1);
    REQUIRE(snapshot.buyPrices[0].orders.size() == 3);

    REQUIRE(snapshot.buyPrices[0].orders[0].orderId == firstId);
    REQUIRE(snapshot.buyPrices[0].orders[1].orderId == secondId);
    REQUIRE(snapshot.buyPrices[0].orders[2].orderId == thirdId);

    REQUIRE(
        snapshot.buyPrices[0].orders[0].sequenceNumber
        < snapshot.buyPrices[0].orders[1].sequenceNumber
    );

    REQUIRE(
        snapshot.buyPrices[0].orders[1].sequenceNumber
        < snapshot.buyPrices[0].orders[2].sequenceNumber
    );
}

TEST_CASE("Snapshot records remaining quantity after partial fill")
{
    OrderBook book;

    auto buyId = book.addOrder(101, Side::BUY, 5000, 10);

    book.addOrder(202, Side::SELL, 5000, 4);

    BookSnapshot snapshot = book.getSnapshot();

    REQUIRE(snapshot.buyPrices.size() == 1);
    REQUIRE(snapshot.buyPrices[0].orders.size() == 1);

    REQUIRE(snapshot.buyPrices[0].orders[0].orderId == buyId);
    REQUIRE(snapshot.buyPrices[0].orders[0].quantity == 6);
}

TEST_CASE("Fully filled order is absent from snapshot")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);
    book.addOrder(202, Side::SELL, 5000, 10);

    BookSnapshot snapshot = book.getSnapshot();

    REQUIRE(snapshot.buyPrices.empty());
    REQUIRE(snapshot.sellPrices.empty());
}

TEST_CASE("Cancelled order is absent from snapshot")
{
    OrderBook book;

    auto buyId = book.addOrder(101, Side::BUY, 5000, 10);
    book.cancelOrder(buyId, 101);

    BookSnapshot snapshot = book.getSnapshot();

    REQUIRE(snapshot.buyPrices.empty());
    REQUIRE(snapshot.sellPrices.empty());
}

TEST_CASE("Snapshot remains frozen after live order book changes")
{
    OrderBook book;

    book.addOrder(101, Side::BUY, 5000, 10);

    BookSnapshot firstSnapshot = book.getSnapshot();

    book.addOrder(102, Side::BUY, 4900, 20);

    BookSnapshot secondSnapshot = book.getSnapshot();

    REQUIRE(firstSnapshot.buyPrices.size() == 1);
    REQUIRE(firstSnapshot.buyPrices[0].price == 5000);

    REQUIRE(secondSnapshot.buyPrices.size() == 2);
    REQUIRE(secondSnapshot.buyPrices[0].price == 5000);
    REQUIRE(secondSnapshot.buyPrices[1].price == 4900);
}