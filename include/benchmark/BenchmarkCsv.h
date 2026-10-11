#pragma once

#include "benchmark/BenchmarkResult.h"

#include <cstdint>
#include <cstddef>
#include <fstream>
#include <vector>

void writeBenchmarkCsv(const std::vector<BenchmarkResult>& results, const std::string& filename);