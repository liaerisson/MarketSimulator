cmake_minimum_required(VERSION 3.20)
project(MarketSimulator LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(orderbook_core
    src/Order.cpp 
    src/OrderBook.cpp
)

target_include_directories(orderbook_core PUBLIC include)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.15.3
)

FetchContent_MakeAvailable(Catch2)

add_executable(orderbook_tests
    tests/OrderTests.cpp
    tests/OrderBookTests.cpp
)

target_link_libraries(orderbook_tests
    PRIVATE
    orderbook_core
    Catch2::Catch2WithMain
)