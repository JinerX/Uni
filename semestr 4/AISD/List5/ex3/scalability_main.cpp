#include <iostream>
#include <vector>
#include <random>
#include "BinomialHeap.hpp"
#include "Experiments.hpp"
#include "Scalability.hpp"

int main() {
    const int n = 500;
    const int numExperiments = 5;
    
    std::cout << "=== Binomial Heap Experiments ===" << std::endl;
    std::cout << "n = " << n << std::endl;
    std::cout << "Number of experiments = " << numExperiments << std::endl << std::endl;
    
    std::vector<ExperimentResult> results;
    
    // Run 5 experiments for n=500
    for (int exp = 1; exp <= numExperiments; exp++) {
        ExperimentResult result = runSingleExperiment(n, exp);
        results.push_back(result);
        
        std::cout << "Experiment " << exp << " completed:" << std::endl;
        std::cout << "  Total comparisons: " << result.totalComparisons << std::endl;
        std::cout << "  Sequence sorted: " << (result.isSorted ? "YES" : "NO") << std::endl;
        std::cout << "  Heap empty: " << (result.heapEmpty ? "YES" : "NO") << std::endl;
        std::cout << std::endl;
    }
    
    // Save results to CSV files
    saveResultsToCSV(results, n);
    saveSummaryToCSV(results, n);
    
    // Print summary statistics
    std::cout << "=== Summary Statistics ===" << std::endl;
    long long totalComparisons = 0;
    long long minComparisons = LLONG_MAX;
    long long maxComparisons = 0;
    
    for (const auto& result : results) {
        totalComparisons += result.totalComparisons;
        minComparisons = std::min(minComparisons, result.totalComparisons);
        maxComparisons = std::max(maxComparisons, result.totalComparisons);
    }
    
    double avgComparisons = (double)totalComparisons / numExperiments;
    
    std::cout << "Average total comparisons: " << std::fixed << std::setprecision(2) << avgComparisons << std::endl;
    std::cout << "Min total comparisons: " << minComparisons << std::endl;
    std::cout << "Max total comparisons: " << maxComparisons << std::endl;
    std::cout << "Difference (max-min): " << (maxComparisons - minComparisons) << std::endl;
    std::cout << "Relative difference: " << std::fixed << std::setprecision(2) 
              << (100.0 * (maxComparisons - minComparisons) / avgComparisons) << "%" << std::endl;
    
    // Run scalability test
    std::cout << std::endl;
    runScalabilityTest();
    
    return 0;
}