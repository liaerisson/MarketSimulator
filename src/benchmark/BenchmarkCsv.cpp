#include "benchmark/BenchmarkCsv.h"

#include <fstream>
#include <vector>

#include "benchmark/BenchmarkCsv.h"

#include <fstream>
#include <stdexcept>

/**
 * @brief Writes raw benchmark results to a CSV file.
 *
 * Each BenchmarkResult is written as one row. Elapsed time is converted to
 * nanoseconds so the CSV contains a simple numeric representation that can
 * be analyzed in spreadsheet or statistical software.
 *
 * @param results Benchmark results to export.
 * @param filename Path or filename of the output CSV file.
 *
 * @throws std::runtime_error If the output file cannot be opened.
 */
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
        // Store elapsed time as an integer number of nanoseconds for later analysis.
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