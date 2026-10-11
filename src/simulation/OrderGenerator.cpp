#include "simulation/OrderGenerator.h"

/**
 * @brief Generates a deterministic sequence of order submissions.
 *
 * Orders are generated from uniform distributions using the supplied seed.
 * Using the same seed and order count produces the same workload, allowing
 * benchmark runs to be reproduced and compared.
 *
 * @param orderCount Number of orders to generate.
 * @param seed Seed used by the pseudorandom number generator.
 * @return Generated order workload.
 */
std::vector<GeneratedOrder> generateOrders(std::size_t orderCount, std::uint32_t seed) {
    std::mt19937 generator(seed);

    // Reserve the full workload size to avoid repeated vector reallocations.
    std::vector<GeneratedOrder> orderList;
    orderList.reserve(orderCount);
    
    std::uniform_int_distribution<std::uint64_t> traderDistribution(1, 100);
    std::uniform_int_distribution<int> sideDistribution(0, 1);
    std::uniform_int_distribution<std::int64_t> priceDistribution(1, 100000);
    std::uniform_int_distribution<std::uint64_t> quantityDistribution(1, 10000);


    for(std::size_t i = 0; i < orderCount; i++) {
        std::uint64_t traderId = traderDistribution(generator);
        Side tradeSide;
        if(sideDistribution(generator) == 0) {
            tradeSide = Side::BUY;
        } else {
            tradeSide = Side::SELL;
        }
        std::int64_t tradePrice = priceDistribution(generator);
        std::uint64_t tradeQuantity = quantityDistribution(generator);

        
        GeneratedOrder newOrder = {traderId, tradeSide, tradePrice, tradeQuantity};
        orderList.push_back(newOrder);
    }

    return orderList;
}


std::vector<std::vector<GeneratedOrder>> generateTrials(std::size_t trialCount, std::size_t ordersPerTrial, std::uint32_t baseSeed) {
    std::vector<std::vector<GeneratedOrder>> generatedTrials;
    generatedTrials.reserve(trialCount);

    for(std::size_t i = 0; i < trialCount; i++) {
        // Each trial uses a reproducible but distinct workload.
        generatedTrials.push_back(generateOrders(ordersPerTrial, baseSeed + i));
    }

    return generatedTrials;
}