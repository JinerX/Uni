#include <iostream>
#include "bipartite.hpp"
#include <vector>
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
    std::vector<int> sets;
    try {
        sets = make_bipartite(adjacency_lists);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "time taken: " << elapsed.count() << std::endl;
        return 1;
    }
    std::cout << "The graph is bipartite" << std::endl;
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time elapsed: " << elapsed.count() << std::endl;

    if (adjacency_lists.size() <= 200) {
        for (int i = 0; i < adjacency_lists.size(); i++) {
            std::cout << "node: " << i+1 << " set: " << sets[i] << std::endl;
        }
    }
}
