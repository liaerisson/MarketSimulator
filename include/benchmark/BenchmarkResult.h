#pragma once

#include <cstdint>
#include <cstddef>
#include <chrono>

/**
 * @brief Raw measurements and metadata from one benchmark trial.
 *
 * BenchmarkResult stores measured values only. Metrics like throughput, speedup, 
 * and scaling efficiency are calculated later during analysis rather than inside the benchmark runner.
 */
struct BenchmarkResult
{
    std::uint32_t seed;
    std::size_t trial;
    std::size_t threadCount;
    std::size_t orderCount;
    std::size_t tradeCount;
    std::chrono::steady_clock::duration elapsed;
};