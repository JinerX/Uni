#if !defined(SIMULATIONS_HPP)
#define SIMULATIONS_HPP

#include <iostream>
#include "Graph.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>

struct BenchmarkResult {
    int n;
    double primAvgTime;
    double kruskalAvgTime;
    double primStdDev;
    double kruskalStdDev;
};


std::vector<double> runBenchmark(CompleteGraph& graph, const std::string& algorithm, int repetitions);
double calculateMean(const std::vector<double>& values);
double calculateStdDev(const std::vector<double>& values, double mean);
void runPerformanceTests();
void runSingleTest();

void runBroadcastExperiments();
void demonstrateBroadcastAlgorithm();

#endif // SIMULATIONS_HPP)