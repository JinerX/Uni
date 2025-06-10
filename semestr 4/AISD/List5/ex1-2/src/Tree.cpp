#include "Tree.hpp"
#include <vector>
#include <algorithm>
#include <functional>
#include <random>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "Graph.hpp"


int computeBroadcastRounds(int n, int root, const std::vector<Edge>& MSTedges) {
    std::vector<std::vector<int>> adj(n);
    for (const auto& edge : MSTedges) {
        adj[edge.from].push_back(edge.to);
        adj[edge.to].push_back(edge.from);
    }

    std::function<int(int, int)> dfs = [&](int node, int parent) -> int {
        std::vector<int> childrenTimes;
        for (int child : adj[node]) {
            if (child == parent) continue;
            childrenTimes.push_back(dfs(child, node));
        }
        
        std::sort(childrenTimes.begin(), childrenTimes.end(), std::greater<int>());
        
        int maxTime = 0;
        for (int i = 0; i < (int)childrenTimes.size(); i++) {
            maxTime = std::max(maxTime, childrenTimes[i] + i + 1);
        }
        
        return maxTime;
    };

    return dfs(root, -1);
}

std::vector<int> analyzeBroadcastForRandomRoots(int n, const std::vector<Edge>& MSTedges, 
                                               int rootChoices, std::mt19937& rng) {
    std::vector<int> results;
    std::uniform_int_distribution<int> dist(0, n - 1);
    
    for (int i = 0; i < rootChoices; i++) {
        int root = dist(rng);
        int rounds = computeBroadcastRounds(n, root, MSTedges);
        results.push_back(rounds);
    }
    
    return results;
}

BroadcastAnalysis calculateBroadcastStatistics(const std::vector<int>& allRounds, int n) {
    BroadcastAnalysis analysis;
    analysis.n = n;
    
    int sum = 0;
    analysis.minRounds = allRounds[0];
    analysis.maxRounds = allRounds[0];
    
    for (int round : allRounds) {
        sum += round;
        analysis.minRounds = std::min(analysis.minRounds, round);
        analysis.maxRounds = std::max(analysis.maxRounds, round);
    }
    
    analysis.avgRounds = static_cast<double>(sum) / allRounds.size();
    
    double sumSquaredDiff = 0.0;
    for (int round : allRounds) {
        double diff = round - analysis.avgRounds;
        sumSquaredDiff += diff * diff;
    }
    analysis.stdDev = std::sqrt(sumSquaredDiff / (allRounds.size() - 1));
    
    return analysis;
}