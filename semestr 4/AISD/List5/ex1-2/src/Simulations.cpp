#include <iostream>
#include "Graph.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include "Simulations.hpp"
#include "Tree.hpp"
#include <vector>

std::vector<double> runBenchmark(CompleteGraph& graph, const std::string& algorithm, int repetitions) {
    std::vector<double> times;
    
    for (int i = 0; i < repetitions; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        
        if (algorithm == "prim") {
            graph.primMST();
        } else if (algorithm == "kruskal") {
            graph.kruskalMST();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        times.push_back(duration.count());
    }
    
    return times;
}

double calculateMean(const std::vector<double>& values) {
    double sum = 0.0;
    for (double val : values) sum += val;
    return sum / values.size();
}

double calculateStdDev(const std::vector<double>& values, double mean) {
    double sum = 0.0;
    for (double val : values) {
        sum += (val - mean) * (val - mean);
    }
    return std::sqrt(sum / (values.size() - 1));
}

void runPerformanceTests() {
    const int nMin = 1000;
    const int nMax = 10000;
    const int step = 1000;
    const int repetitions = 10;
    
    std::vector<BenchmarkResult> results;
    std::ofstream csvFile("benchmark_results.csv");
    
    csvFile << "n,prim_avg_time,kruskal_avg_time,prim_std_dev,kruskal_std_dev\n";
    
    std::cout << "=== TESTY WYDAJNOŚCI ALGORYTMÓW MST ===\n";
    std::cout << "Parametry: n = [" << nMin << ", " << nMax << "], krok = " << step 
              << ", powtórzenia = " << repetitions << "\n\n";
    
    for (int n = nMin; n <= nMax; n += step) {
        std::cout << "Testowanie dla n = " << n << "..." << std::flush;
        
        CompleteGraph graph(n);
        
        // prim
        std::vector<double> primTimes = runBenchmark(graph, "prim", repetitions);
        double primMean = calculateMean(primTimes);
        double primStdDev = calculateStdDev(primTimes, primMean);
        
        // kruskal
        std::vector<double> kruskalTimes = runBenchmark(graph, "kruskal", repetitions);
        double kruskalMean = calculateMean(kruskalTimes);
        double kruskalStdDev = calculateStdDev(kruskalTimes, kruskalMean);
        
        BenchmarkResult result = {n, primMean, kruskalMean, primStdDev, kruskalStdDev};
        results.push_back(result);
        
        csvFile << n << "," << primMean << "," << kruskalMean << "," 
                << primStdDev << "," << kruskalStdDev << "\n";
        
        std::cout << " OK (Prim: " << std::fixed << std::setprecision(1) 
                  << primMean << "μs, Kruskal: " << kruskalMean << "μs)\n";
    }
    
    csvFile.close();
    
    std::cout << "\n=== PODSUMOWANIE WYNIKÓW ===\n";
    std::cout << std::setw(5) << "n" << std::setw(15) << "Prim (μs)" 
              << std::setw(15) << "Kruskal (μs)" << std::setw(15) << "Stosunek K/P" << "\n";
    std::cout << std::string(50, '-') << "\n";
    
    for (const auto& result : results) {
        double ratio = result.kruskalAvgTime / result.primAvgTime;
        std::cout << std::setw(5) << result.n 
                  << std::setw(15) << std::fixed << std::setprecision(1) << result.primAvgTime
                  << std::setw(15) << result.kruskalAvgTime
                  << std::setw(15) << std::setprecision(2) << ratio << "\n";
    }
}

void runSingleTest() {
    int n;
    
    std::cout << "=== Generator grafu pełnego z losowymi wagami ===\n";
    std::cout << "Podaj liczbę wierzchołków (n): ";
    std::cin >> n;

    if (n <= 0) {
        std::cerr << "Błąd: Liczba wierzchołków musi być większa od 0!\n";
        return;
    }

    CompleteGraph graph(n);

    std::cout << "\n" << std::string(50, '=') << "\n";
    
    std::cout << "Wygenerowano graf pełny:\n";
    std::cout << "- Liczba wierzchołków: " << graph.getVertexCount() << "\n";
    std::cout << "- Liczba krawędzi: " << graph.getEdgeCount() << "\n";
    std::cout << "- Wagi z przedziału (0, 1)\n\n";

    if (n <= 20) {
        graph.printGraph();
    } else {
        std::cout << "Graf jest zbyt duży do wyświetlenia macierzy sąsiedztwa.\n";
    }

    // Opcjonalne wyświetlenie listy krawędzi
    if (n <= 8) {
        graph.printEdgeList();
    }

    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "WYZNACZANIE MSP\n";
    std::cout << std::string(70, '=') << "\n";

    // Algorytm Prima
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Edge> primMST = graph.primMST();
    auto end = std::chrono::high_resolution_clock::now();
    auto primTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    graph.printMST(primMST, "Algorytm Prima");
    std::cout << "Czas wykonania: " << primTime.count() << " mikrosekund\n";

    // Algorytm Kruskala
    start = std::chrono::high_resolution_clock::now();
    std::vector<Edge> kruskalMST = graph.kruskalMST();
    end = std::chrono::high_resolution_clock::now();
    auto kruskalTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    graph.printMST(kruskalMST, "Algorytm Kruskala");
    std::cout << "Czas wykonania: " << kruskalTime.count() << " mikrosekund\n";

    // Porównanie wyników
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "PORÓWNANIE ALGORYTMÓW:\n";
    
    double primWeight = 0.0, kruskalWeight = 0.0;
    for (const Edge& e : primMST) primWeight += e.weight;
    for (const Edge& e : kruskalMST) kruskalWeight += e.weight;
    
    std::cout << "Prima - waga MST: " << std::fixed << std::setprecision(6) << primWeight << "\n";
    std::cout << "Kruskal - waga MST: " << std::fixed << std::setprecision(6) << kruskalWeight << "\n";
    std::cout << "Różnica: " << std::abs(primWeight - kruskalWeight) << "\n";
    std::cout << "Prima - czas: " << primTime.count() << " μs\n";
    std::cout << "Kruskal - czas: " << kruskalTime.count() << " μs\n";
}


void runBroadcastExperiments() {
    const int nMin = 10;
    const int nMax = 200;
    const int step = 10;
    const int trials = 20;
    const int rootChoices = 15;
    
    std::vector<BroadcastAnalysis> results;
    std::ofstream csvFile("broadcast_analysis.csv");
    
    csvFile << "n,avg_rounds,min_rounds,max_rounds,std_dev\n";
    
    std::cout << "=== ANALIZA ROZESŁANIA INFORMACJI W DRZEWIE ===\n";
    std::cout << "Parametry: n = [" << nMin << ", " << nMax << "], krok = " << step << "\n";
    std::cout << "Trials per n = " << trials << ", Root choices per trial = " << rootChoices << "\n\n";
    
    std::mt19937 rng(std::random_device{}());
    
    for (int n = nMin; n <= nMax; n += step) {
        std::cout << "Analizowanie dla n = " << n << "..." << std::flush;
        
        std::vector<int> allRounds;
        
        for (int trial = 1; trial <= trials; ++trial) {
            CompleteGraph graph(n);
            
            std::vector<Edge> MSTedges;
            if (trial % 2 == 0) {
                MSTedges = graph.primMST();
            } else {
                MSTedges = graph.kruskalMST();
            }
            
            std::vector<int> trialRounds = analyzeBroadcastForRandomRoots(n, MSTedges, rootChoices, rng);
            allRounds.insert(allRounds.end(), trialRounds.begin(), trialRounds.end());
        }
        
        BroadcastAnalysis analysis = calculateBroadcastStatistics(allRounds, n);
        results.push_back(analysis);
        
        csvFile << n << "," << analysis.avgRounds << "," 
                << analysis.minRounds << "," << analysis.maxRounds << "," 
                << analysis.stdDev << "\n";
        
        std::cout << " OK (Avg: " << std::fixed << std::setprecision(2) 
                  << analysis.avgRounds << ", Min: " << analysis.minRounds 
                  << ", Max: " << analysis.maxRounds << ", Samples: " << allRounds.size() << ")\n";
    }
    
    csvFile.close();
    
    std::cout << "\n=== PODSUMOWANIE ANALIZY ROZESŁANIA ===\n";
    std::cout << std::setw(5) << "n" << std::setw(10) << "Avg" 
              << std::setw(8) << "Min" << std::setw(8) << "Max" 
              << std::setw(10) << "StdDev" << std::setw(10) << "Samples" << "\n";
    std::cout << std::string(55, '-') << "\n";
    
    for (const auto& result : results) {
        int totalSamples = trials * rootChoices;
        std::cout << std::setw(5) << result.n 
                  << std::setw(10) << std::fixed << std::setprecision(2) << result.avgRounds
                  << std::setw(8) << result.minRounds
                  << std::setw(8) << result.maxRounds
                  << std::setw(10) << std::setprecision(3) << result.stdDev 
                  << std::setw(10) << totalSamples << "\n";
    }
    
    std::cout << "\nWyniki zapisano do pliku:\n";
    std::cout << "- broadcast_analysis.csv (dane do wykresów)\n";
}

void demonstrateBroadcastAlgorithm() {
    int n;
    std::cout << "=== DEMONSTRACJA ALGORYTMU ROZESŁANIA ===\n";
    std::cout << "Podaj liczbę wierzchołków drzewa: ";
    std::cin >> n;
    
    if (n <= 0) {
        std::cerr << "Błąd: Liczba wierzchołków musi być większa od 0!\n";
        return;
    }
    
    // Generate MST
    CompleteGraph graph(n);
    std::vector<Edge> mst = graph.primMST();
    
    std::cout << "\nWygenerowane MST:\n";
    for (const Edge& edge : mst) {
        std::cout << "(" << edge.from << ", " << edge.to << ") - waga: " 
                  << std::fixed << std::setprecision(3) << edge.weight << "\n";
    }
    
    std::vector<int> allRounds;
    for (int root = 0; root < n; root++) {
        int rounds = computeBroadcastRounds(n, root, mst);
        allRounds.push_back(rounds);
        std::cout << "Korzeń " << root << ": " << rounds << " rund\n";
    }
    
    auto minIt = std::min_element(allRounds.begin(), allRounds.end());
    int optimalRoot = std::distance(allRounds.begin(), minIt);
    
    std::cout << "\n=== ANALIZA WSZYSTKICH KORZENI ===\n";
    BroadcastAnalysis analysis = calculateBroadcastStatistics(allRounds, n);
    std::cout << "Średnia liczba rund: " << std::fixed << std::setprecision(2) << analysis.avgRounds << "\n";
    std::cout << "Minimalna liczba rund: " << analysis.minRounds << " (korzeń " << optimalRoot << ")\n";
    std::cout << "Maksymalna liczba rund: " << analysis.maxRounds << "\n";
    std::cout << "Odchylenie standardowe: " << std::setprecision(3) << analysis.stdDev << "\n";
}