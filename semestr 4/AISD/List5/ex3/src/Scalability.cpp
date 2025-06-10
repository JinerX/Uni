#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include "BinomialHeap.hpp"
#include "Experiments.hpp"
#include "Scalability.hpp"

void runScalabilityTest() {
    std::cout << "=== Scalability Test ===" << std::endl;
    
    std::vector<int> n_values;
    for (int n = 100; n <= 10000; n += 100) {
        n_values.push_back(n);
    }
    
    std::ofstream file("scalability_results.csv");
    file << "n,avg_total_comparisons,avg_per_n,std_dev" << std::endl;
    
    for (int n : n_values) {
        std::cout << "Testing n = " << n << "..." << std::endl;
        
        std::vector<long long> totalComparisons;
        const int numRuns = 3;
        
        for (int run = 1; run <= numRuns; run++) {
            ExperimentResult result = runSingleExperiment(n, run);
            totalComparisons.push_back(result.totalComparisons);
        }
        
        long long sum = 0;
        for (long long comp : totalComparisons) {
            sum += comp;
        }
        double avgTotal = (double)sum / numRuns;
        double avgPerN = avgTotal / n;
        
        double variance = 0;
        for (long long comp : totalComparisons) {
            variance += (comp - avgTotal) * (comp - avgTotal);
        }
        double stdDev = sqrt(variance / numRuns);
        
        file << n << "," << std::fixed << std::setprecision(2) 
             << avgTotal << "," << avgPerN << "," << stdDev << std::endl;
        
        std::cout << "  n=" << n << ": avg=" << avgTotal 
                  << ", avg/n=" << avgPerN << std::endl;
    }
    
    file.close();
    std::cout << "Scalability results saved to scalability_results.csv" << std::endl;
}