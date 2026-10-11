#pragma once

#include <cstdint>
#include <cstddef>
#include <chrono>

struct BenchmarkResult
{
    std::uint32_t seed;
    std::size_t trial;
    std::size_t threadCount;
    std::size_t orderCount;
    std::size_t tradeCount;
    std::chrono::steady_clock::duration elapsed;
};