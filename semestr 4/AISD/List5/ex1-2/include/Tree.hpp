#ifndef TREE_HPP
#define TREE_HPP

#include <vector>
#include <random>
#include "Graph.hpp"

struct BroadcastAnalysis {
    int n;
    double avgRounds;
    int minRounds;
    int maxRounds;
    double stdDev;
};

// Core broadcast algorithm
int computeBroadcastRounds(int n, int root, const std::vector<Edge>& MSTedges);

// Analysis functions
std::vector<int> analyzeBroadcastForRandomRoots(int n, const std::vector<Edge>& MSTedges, 
                                               int rootChoices, std::mt19937& rng);

BroadcastAnalysis calculateBroadcastStatistics(const std::vector<int>& allRounds, int n);

#endif // TREE_HPP