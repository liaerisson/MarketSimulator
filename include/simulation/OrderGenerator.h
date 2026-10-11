#pragma once

#include "GeneratedOrder.h"
#include <cstdint>
#include <vector>
#include <random>

std::vector<GeneratedOrder> generateOrders(std::size_t orderCount, std::uint32_t seed);

std::vector<std::vector<GeneratedOrder>> generateTrials(std::size_t trialCount, std::size_t ordersPerTrial, std::uint32_t baseSeed);