cmake_minimum_required(VERSION 3.20)
project(MarketSimulator LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(orderbook_core
    src/Order.cpp 
    src/OrderBook.cpp
    src/display/DebugDisplay.cpp
    src/display/TraderDisplay.cpp
)

target_include_directories(orderbook_core PUBLIC include)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.15.3
)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

include(CTest)
include(Catch)

add_executable(orderbook_tests
    tests/OrderTests.cpp
    tests/OrderBookTests.cpp
    tests/TradeTests.cpp
    tests/DisplayTests.cpp
    tests/SnapshotTests.cpp
)

add_executable(orderbook_demo
    src/main.cpp
)

target_link_libraries(orderbook_tests
    PRIVATE
    orderbook_core
    Catch2::Catch2WithMain
)

target_link_libraries(orderbook_demo
    PRIVATE
    orderbook_core
)

catch_discover_tests(orderbook_tests)