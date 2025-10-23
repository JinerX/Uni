#include <iostream>
#include <vector>
#include "searches.hpp"



int main(int argc, char** argv) {
    std::vector<std::vector<int>> adjacency_lists = {
        {1,2},
        {2,3,4},
        {4,5},
        {4},
        {5},
        {}
    };

    std::cout << "==========DFS=========" << std::endl;
    DFS(adjacency_lists, 0, true);
    std::cout << "==========BFS=========" << std::endl;
    BFS(adjacency_lists, 0, true);
    return 0;
}