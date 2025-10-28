#include "load_data.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <tuple>

std::tuple<std::vector<std::vector<int>>, bool> load_standard(int argc, char** argv) {
    bool display_tree = false;
    int n;
    int m;
    int start_edges;
    if (std::string(argv[2]) == "-t") {
        display_tree = true;
        n = std::stoi(argv[3]);
        m = std::stoi(argv[4]);
        start_edges = 5;
    }
    else {
        n = std::stoi(argv[2]);
        m = std::stoi(argv[3]);
        start_edges = 4;
    }
    std::vector<std::vector<int>> adjacency_list(n);
    for (int i = start_edges; i < argc; i += 2) {
        int v = std::stoi(argv[i]) - 1;
        int u = std::stoi(argv[i+1]) - 1;
        adjacency_list[v].push_back(u);
    }
    return {adjacency_list, display_tree};
}

std::tuple<std::vector<std::vector<int>>, bool> load_textfile(std::string filepath) {
    std::ifstream file(filepath);

    std::string x;
    std::string directed;
    bool display_tree = false;
    file >> directed; // directed or undirected
    file >> x; // potential -t flag
    int n;
    if (x == "-t") {
        display_tree = true;
        file >> x; // n
        n = std::stoi(x);
    }
    else {
        n = std::stoi(x);
    }
    
    std::vector<std::vector<int>> adjacency_list(n);

    file >> x; // m
    int m = std::stoi(x);
    std::string y;
    while (file >> x) {
        file >> y;
        int from = std::stoi(x) - 1;
        int to = std::stoi(y) - 1;
        
        adjacency_list[from].push_back(to);
        if (directed != "D") {
            adjacency_list[to].push_back(from);
        }

    }
    return {adjacency_list, display_tree};
}
