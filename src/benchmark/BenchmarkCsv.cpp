#include "benchmark/BenchmarkCsv.h"

#include <fstream>
#include <vector>

#include "benchmark/BenchmarkCsv.h"

#include <fstream>
#include <stdexcept>

void writeBenchmarkCsv(const std::vector<BenchmarkResult> &results, const std::string &filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open benchmark CSV file.");
    }

    file << "trial,seed,threads,orders,trades,elapsed_ns\n";

    for (const BenchmarkResult& result : results)
    {
        auto elapsedNs = std::chrono::duration_cast<std::chrono::nanoseconds>(result.elapsed);
        
        file << result.trial << ","
             << result.seed << ","
             << result.threadCount << ","
             << result.orderCount << ","
             << result.tradeCount << ","
             << elapsedNs.count()
             << "\n";
    }
}