#pragma once

#include "benchmark/BenchmarkResult.h"
#include "simulation/GeneratedOrder.h"
#include <cstdint>
#include <cstddef>
#include <vector>

BenchmarkResult runBenchmark(const std::vector<GeneratedOrder>& workload, std::uint32_t seed, std::size_t trial, std::size_t threadCount);

std::vector<BenchmarkResult> runBenchmarkTrials(const std::vector<std::vector<GeneratedOrder>>& trials, std::uint32_t baseSeed, std::size_t threadCount);