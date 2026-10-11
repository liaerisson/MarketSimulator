#include "simulation/OrderGenerator.h"
#include "benchmark/BenchmarkRunner.h"
#include "benchmark/BenchmarkCsv.h"

#include <cstddef>
#include <cstdint>
#include <vector>

/**
 * @brief Entry point for the performance benchmark.
 *
 * Generates deterministic workloads, runs each workload through the
 * OrderBook, and exports the raw measurements to CSV.
 *
 * The current configuration establishes the single-thread Release baseline
 * that later multithreaded benchmarks will be compared against.
 */
int main()
{
    // Baseline benchmark configuration.
    std::size_t trialCount = 10;
    std::size_t ordersPerTrial = 1000000;
    std::uint32_t baseSeed = 1000;
    std::size_t threadCount = 1;
    
    std::vector<std::vector<GeneratedOrder>> trials = generateTrials(trialCount, ordersPerTrial, baseSeed);
    std::vector<BenchmarkResult> trialResults = runBenchmarkTrials(trials, baseSeed, threadCount);

    writeBenchmarkCsv(trialResults, "single_thread_benchmark.csv");
    return 0;
}