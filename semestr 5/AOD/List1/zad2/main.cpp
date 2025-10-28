#include <iostream>
#include <vector>
#include "Kahn.hpp"
#include "load_data.hpp"
#include <tuple>
#include <chrono>

void display_adj(std::vector<std::vector<int>> al) {
    for (int i = 0; i < al.size(); i++) {
        std::cout << "DISPLAYING NEIGHBORS OF NODE " << i << std::endl;
        for (int j = 0; j < al[i].size(); j++) {
            std::cout << al[i][j] << std::endl;
        }
    }
}


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
    
    std::vector<int> top_ord;
    try
    {
        top_ord = Kahn(adjacency_lists);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "time taken: " << elapsed.count() << std::endl;
        return 1;
    }
    std::cout << "The graph contains no cycles" << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time elapsed: " << elapsed.count() << std::endl;

    

    
    if (adjacency_lists.size() <= 200) {
        std::cout << "Printing topological ordering" << std::endl;
        for (int i = 0; i < top_ord.size(); i++) {
            std::cout << i << "th element: " << top_ord[i] << std::endl;
        }
    }
}
