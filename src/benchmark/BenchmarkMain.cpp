#include "simulation/OrderGenerator.h"
#include "benchmark/BenchmarkRunner.h"
#include "benchmark/BenchmarkCsv.h"

#include <cstddef>
#include <cstdint>
#include <vector>

int main()
{
    std::size_t trialCount = 10;
    std::size_t ordersPerTrial = 1000000;
    std::uint32_t baseSeed = 1000;
    std::size_t threadCount = 1;
    
    std::vector<std::vector<GeneratedOrder>> trials = generateTrials(trialCount, ordersPerTrial, baseSeed);
    std::vector<BenchmarkResult> trialResults = runBenchmarkTrials(trials, baseSeed, threadCount);

    writeBenchmarkCsv(trialResults, "single_thread_benchmark.csv");
    return 0;
}