#include <iostream>
#include <vector>
#include "searches.hpp"
#include "load_data.hpp"
#include <string>
#include <tuple>
#include <chrono>

void display_adj(std::vector<std::vector<int>> al) {
    std::cout << "dupadupa" << std::endl;
    for (int i = 0; i < al.size(); i++) {
        std::cout << "DISPLAYING NEIGHBORS OF NODE " << i << std::endl;
        for (int j = 0; j < al[i].size(); j++) {
            std::cout << al[i][j] << std::endl;
        }
    }
}

int main(int argc, char** argv) {

    bool display_tree = false;
    std::vector<std::vector<int>> adjacency_list;
    if (argc == 2) {
        std::tie(adjacency_list, display_tree) = load_textfile(argv[1]);
    }
    else {
        std::tie(adjacency_list, display_tree) = load_standard(argc, argv);
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "==========DFS=========" << std::endl;
    DFS(adjacency_list, 0, display_tree);
    std::cout << "==========BFS=========" << std::endl;
    BFS(adjacency_list, 0, display_tree);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "time taken: " << elapsed.count() << std::endl;
    return 0;
}