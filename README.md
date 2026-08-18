# C++ Market Simulator

A C++ limit order book and matching engine built to explore market microstructure, systems programming, and performance-oriented design.

The project currently supports order placement, cancellation, price-time priority, and matching between buy and sell orders. It is being developed incrementally toward a larger market simulation with concurrent order flow, performance benchmarking, and trader behavior.

## Current Features

* Buy and sell limit orders
* Price-time priority
* Separate ordered bid and ask books
* FIFO execution within each price level
* Partial and full order fills
* Order cancellation with trader ownership validation
* Fast order lookup by ID
* Best bid and best ask inspection
* Active-order quantity lookup
* Automatic cleanup of filled orders and empty price levels
* Integer-based price representation to avoid floating-point comparison issues
* Catch2 test suite covering validation, cancellation, matching, FIFO behavior, price priority, and price-boundary behavior

## Architecture

Each side of the order book is organized by price level:

* Sell orders are stored in ascending price order
* Buy orders are stored in descending price order
* Orders at the same price are stored in insertion order

The implementation currently uses:

* `std::map` for ordered price levels
* `std::list` for FIFO order storage and stable iterators
* `std::unordered_map` for direct order lookup during cancellation and matching
* `std::optional` for read-only queries that may not have a value, such as best bid/ask on an empty book

Each order contains:

* Order ID
* Trader ID
* Side
* Price
* Remaining quantity
* Sequence number

Only quantity is mutable after an order is created.

## Matching

Incoming orders attempt to match against the best available price on the opposite side of the book.

A buy order may execute against asks priced at or below its limit price, while a sell order may execute against bids priced at or above its limit price.

Price priority is applied before time priority. At the same price level, orders are matched FIFO.

When quantities differ, the smaller quantity is executed. A partially filled resting order remains active with its reduced quantity, while any unfilled portion of an incoming order is added to the book.

## Build and Test

The project uses CMake and Catch2.

From the repository root on Windows with an x64 toolchain:

```powershell
cmake -S . -B build -A x64
cmake --build build
ctest --test-dir build
```

The current test suite holds 32 passing tests covering core `Order` and `OrderBook` behavior.

## Development Status

This project is actively under development.

### Implemented

* [x] Order representation and validation
* [x] Ordered buy and sell price levels
* [x] Price-time priority
* [x] Order lookup
* [x] Cancellation
* [x] Matching
* [x] Partial and full fills
* [x] Best bid / best ask inspection
* [x] Catch2 unit and integration tests
* [x] CTest integration

### Next

* [ ] Trade/execution records
* [ ] Order book display and simulation output
* [ ] Synthetic trader/order generation
* [ ] Performance profiling and benchmarking
* [ ] Multithreaded order flow
* [ ] Synchronization and concurrency testing
* [ ] Market statistics and microstructure experiments

## Goals

The main goal of the project is to develop a deeper understanding of:

* Modern C++
* STL containers and iterator behavior
* Data-structure tradeoffs
* Matching-engine design
* Market microstructure
* Multithreading and synchronization
* Performance measurement and optimization

The eventual simulator will be used to explore how different trader behaviors and market conditions affect liquidity, spreads, execution, and price formation.