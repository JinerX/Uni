#include <iostream>
#include <vector>
#include "tarjan.hpp"
#include <set>
#include "load_data.hpp"
#include <chrono>


int main(int argc, char** argv){
    std::vector<std::vector<int>> adjacency_lists;
    bool _;
    if (argc == 2) {
        std::tie(adjacency_lists, _) = load_textfile(argv[1]);
    }
    else {
        std::tie(adjacency_lists, _) = load_standard(argc, argv);
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::set<std::set<int>> SCCs = tarjan(adjacency_lists);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time elapsed: " << elapsed.count() << std::endl;


    std::cout << "Number of components: " << SCCs.size() << std::endl;
    
    int n = adjacency_lists.size();
    int i = 0;
    for (auto SCC : SCCs) {
        std::cout << "Component number: " << i << " number of elements: " << SCC.size() << std::endl;
        i++;
        if (n <= 200) {
            std::cout << "List of verticies" << std::endl;
            for (auto vertex : SCC) {
                std::cout << vertex << " ";
            }
            std::cout << std::endl;
        }
    }
}
