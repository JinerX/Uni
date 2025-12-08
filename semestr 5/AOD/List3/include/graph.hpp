#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <tuple>
#include <vector>

class Graph {
public:
    Graph();
    std::vector<std::vector<std::tuple<int,int>>> adjacency_list; // adjacency_list[i][0] - what vertex, [1] - what weight
    int n; // number of verticies
    int e; // number of edges
    int c; // max edge weight
};

#endif