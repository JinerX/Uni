#ifndef LOCAL_SEARCH_HPP
#define LOCAL_SEARCH_HPP

#include "tsp.hpp"

#include <random>
#include <vector>

struct SearchResult {
    std::vector<int> tour;
    int cost = 0;
    int improvement_steps = 0;
};

std::vector<int> random_permutation(int n, std::mt19937& rng);

SearchResult local_search_invert_best_improvement(
    const TspInstance& instance,
    std::vector<int> tour
);

SearchResult local_search_invert_random_sample(
    const TspInstance& instance,
    std::vector<int> tour,
    std::mt19937& rng
);

#endif