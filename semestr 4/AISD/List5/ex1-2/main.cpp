#include <iostream>
#include "Graph.hpp"
#include <chrono>
#include "Simulations.hpp"

int main(int argc, char** argv) {
    std::cout << "Wybierz tryb:\n";
    std::cout << "1. Pojedynczy test z wizualizacją MST\n";
    std::cout << "2. Testy wydajności MST (benchmark)\n";
    std::cout << "3. Demonstracja algorytmu rozesłania informacji\n";
    std::cout << "4. Analiza eksperymentalna rozesłania informacji\n";
    std::cout << "Wybór (1/2/3/4): ";
    
    int choice;
    std::cin >> choice;
    
    if (choice == 1) {
        runSingleTest();
    } else if (choice == 2) {
        runPerformanceTests();
    } else if (choice == 3) {
        demonstrateBroadcastAlgorithm();
    } else if (choice == 4) {
        runBroadcastExperiments();
    } else {
        std::cerr << "Nieprawidłowy wybór!\n";
        return 1;
    }

    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "Program zakończony pomyślnie.\n";

    return 0;
}