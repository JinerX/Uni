#if !defined(EXPERIMENTS_HPP)
#define EXPERIMENTS_HPP

#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <iomanip>
#include "BinomialHeap.hpp"

struct ExperimentResult {
    std::vector<long long> operationComparisons;
    std::vector<long long> cumulativeComparisons;
    long long totalComparisons;
    bool isSorted;
    bool heapEmpty;
};

void saveSummaryToCSV(const std::vector<ExperimentResult>& results, int n);
void saveResultsToCSV(const std::vector<ExperimentResult>& results, int n);
ExperimentResult runSingleExperiment(int n, int experimentNum);


#endif // EXPERIMENTS_HPP)
