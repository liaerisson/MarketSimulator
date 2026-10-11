#include "benchmark/BenchmarkRunner.h"
#include "OrderBook.h"

#include <chrono>

/**
 * @brief Runs one benchmark trial over a pre-generated order workload.
 *
 * Processes the supplied orders through a fresh OrderBook and measures only
 * the time spent submitting and matching those orders. Workload generation,
 * result collection, and reporting are excluded from the timed section.
 *
 * @param workload Pre-made orders used for this trial.
 * @param seed Seed associated with the workload.
 * @param trial Index identifying this benchmark trial.
 * @param threadCount Number of threads used to process the workload.
 *
 * @return BenchmarkResult containing the workload metadata, trade count,
 *         and measured execution duration.
 *
 * @note threadCount should always match the number of threads actually used
 *       by the benchmark implementation.
 */
BenchmarkResult runBenchmark(const std::vector<GeneratedOrder>& workload, std::uint32_t seed, std::size_t trial, std::size_t threadCount) {
    OrderBook book;
    auto start = std::chrono::steady_clock::now();
    
    for (const GeneratedOrder& order : workload)
    {
        book.addOrder(order.traderId, order.side, order.price, order.quantity);
    }
    
    auto end = std::chrono::steady_clock::now();

    auto elapsed = end - start;
    std::size_t tradeCount = book.getTrades().size();

    BenchmarkResult result = {seed, trial, threadCount, workload.size(), tradeCount, elapsed};
    return result;
}

/**
 * @brief Runs the same benchmark configuration across multiple generated trials.
 *
 * Executes each stored workload using its corresponding seed and collects the resulting benchmark measurements.
 *
 * @param trials Pre-made workloads to benchmark.
 * @param baseSeed Base seed used to reconstruct each trial's seed.
 * @param threadCount Number of threads used for each benchmark run.
 * @return Benchmark results for all trials.
 */
std::vector<BenchmarkResult> runBenchmarkTrials(const std::vector<std::vector<GeneratedOrder>>& trials, std::uint32_t baseSeed, std::size_t threadCount) {
    std::vector<BenchmarkResult> results;
    results.reserve(trials.size());

    for(std::size_t i = 0; i < trials.size(); i++) {
        BenchmarkResult result = runBenchmark(trials[i], baseSeed + i, i, threadCount);
        results.push_back(result);
    }

    return results;
}