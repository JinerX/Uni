#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <iomanip>
#include "BinomialHeap.hpp"
#include "Experiments.hpp"
#include <string>

ExperimentResult runSingleExperiment(int n, int experimentNum) {
    std::cout << "Running experiment " << experimentNum << "..." << std::endl;
    
    std::random_device rd;
    std::mt19937 gen(rd() + experimentNum);
    std::uniform_int_distribution<> dis(1, 10000);
    
    BinomialHeap::resetComparisonCount();
    
    std::vector<long long> allOperationComparisons;
    std::vector<long long> allCumulativeComparisons;
    
    // Step 1: Create two empty heaps
    BinomialHeap H1, H2;
    
    // Step 2: Insert n random elements into each heap
    // Insert into H1
    for (int i = 0; i < n; i++) {
        long long beforeInsert = BinomialHeap::getComparisonCount();
        H1.insert(dis(gen));
        long long afterInsert = BinomialHeap::getComparisonCount();
        
        allOperationComparisons.push_back(afterInsert - beforeInsert);
        allCumulativeComparisons.push_back(afterInsert);
    }
    
    // Insert into H2
    for (int i = 0; i < n; i++) {
        long long beforeInsert = BinomialHeap::getComparisonCount();
        H2.insert(dis(gen));
        long long afterInsert = BinomialHeap::getComparisonCount();
        
        allOperationComparisons.push_back(afterInsert - beforeInsert);
        allCumulativeComparisons.push_back(afterInsert);
    }
    
    // Step 3: Union the two heaps
    BinomialHeap H;
    long long beforeUnion = BinomialHeap::getComparisonCount();
    H.unionWith(H1);
    H.unionWith(H2);
    long long afterUnion = BinomialHeap::getComparisonCount();
    
    allOperationComparisons.push_back(afterUnion - beforeUnion);
    allCumulativeComparisons.push_back(afterUnion);
    
    // Step 4: Extract 2n elements and track comparisons
    std::vector<int> extractedElements;
    
    for (int i = 0; i < 2*n; i++) {
        long long beforeExtract = BinomialHeap::getComparisonCount();
        int minElement = H.extractMin();
        long long afterExtract = BinomialHeap::getComparisonCount();
        
        extractedElements.push_back(minElement);
        allOperationComparisons.push_back(afterExtract - beforeExtract);
        allCumulativeComparisons.push_back(afterExtract);
    }
    
    // Verify sorting
    bool isSorted = true;
    for (int i = 1; i < extractedElements.size(); i++) {
        if (extractedElements[i] < extractedElements[i-1]) {
            isSorted = false;
            break;
        }
    }
    
    ExperimentResult result;
    result.operationComparisons = allOperationComparisons;
    result.cumulativeComparisons = allCumulativeComparisons;
    result.totalComparisons = BinomialHeap::getComparisonCount();
    result.isSorted = isSorted;
    result.heapEmpty = H.isEmpty();
    
    return result;
}

void saveResultsToCSV(const std::vector<ExperimentResult>& results, int n) {
    std::ofstream file("binomial_heap_results_n" + std::to_string(n) + ".csv");
    
    file << "Operation";
    for (int i = 0; i < results.size(); i++) {
        file << ",Experiment_" << (i+1) << "_Comparisons";
        file << ",Experiment_" << (i+1) << "_Cumulative";
    }
    file << std::endl;
    
    int maxOperations = 0;
    for (const auto& result : results) {
        maxOperations = std::max(maxOperations, (int)result.operationComparisons.size());
    }
    
    for (int op = 0; op < maxOperations; op++) {
        file << (op + 1);
        for (const auto& result : results) {
            if (op < result.operationComparisons.size()) {
                file << "," << result.operationComparisons[op];
                file << "," << result.cumulativeComparisons[op];
            } else {
                file << ",,";
            }
        }
        file << std::endl;
    }
    
    file.close();
    std::cout << "Results saved to binomial_heap_results_n" << n << ".csv" << std::endl;
}

void saveSummaryToCSV(const std::vector<ExperimentResult>& results, int n) {
    std::ofstream file("binomial_heap_summary_n" + std::to_string(n) + ".csv");
    
    file << "Experiment,Total_Comparisons,Average_Per_Operation,Is_Sorted,Heap_Empty" << std::endl;
    
    for (int i = 0; i < results.size(); i++) {
        const auto& result = results[i];
        double avgPerOp = (double)result.totalComparisons / (4*n + 1);
        
        file << (i+1) << "," 
             << result.totalComparisons << ","
             << std::fixed << std::setprecision(3) << avgPerOp << ","
             << (result.isSorted ? "YES" : "NO") << ","
             << (result.heapEmpty ? "YES" : "NO") << std::endl;
    }
    
    file.close();
    std::cout << "Summary saved to binomial_heap_summary_n" << n << ".csv" << std::endl;
}