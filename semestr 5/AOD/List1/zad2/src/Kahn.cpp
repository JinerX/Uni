#include <vector>
#include <iostream>
#include "Kahn.hpp"
#include <set>


std::vector<int> Kahn(std::vector<std::vector<int>> adjacency_list) {
    // Kahn's algorithm for topological sort
    //1. Locate nodes which have no incoming edges (if no such node  cycle return error)
    //2. Add those nodes to a set
    //3. while set is not empty get node from set, append it to a list, for each m - neighbor of n remove edge (n,m) if m has no edges insert it into the set
    //4. if graph still has edges it contains a cycle


    // Locate nodes with no incoming edges

    //create a reverse_adjacency_list - list of incoming edges for a given node
    std::vector<std::set<int>> incoming_edges(adjacency_list.size());
    
    std::set<int> candidates;
    for (int i = 0; i < adjacency_list.size(); i++) {
        candidates.insert(i);
    }

    for (int i = 0; i < adjacency_list.size(); i++) {
        for (int j = 0; j < adjacency_list[i].size(); j++) {
            incoming_edges[adjacency_list[i][j]].insert(i);
            candidates.erase(adjacency_list[i][j]);
        }
    }

    /*
    for (int i = 0; i < incoming_edges.size(); i++) {
        std::cout << "Vertex: " << i << std::endl;
        std::cout << "Incoming edges: "  << std::endl;
        for (auto edge : incoming_edges[i]) {
            std::cout << "Incoming Edge: " << edge << std::endl;
        }
    }
    */

    std::vector<int> topological_ordering;

    while (!candidates.empty()) {
        int current = *candidates.begin();
        candidates.erase(current);
        // std::cout << "current: " << current << std::endl;
        topological_ordering.push_back(current);
        int n = adjacency_list[current].size();
        auto neighbors = adjacency_list[current];
        for (auto neighbor : neighbors) {
            incoming_edges[neighbor].erase(current);
            if (incoming_edges[neighbor].empty()) {
                candidates.insert(neighbor);
            }
            // adjacency_list[current].erase(adjacency_list[current].begin() + i);
        }
    }
    for (int i = 0; i < incoming_edges.size(); i++) {
        if (!incoming_edges[i].empty()) {
            throw std::logic_error("The given graph has a cycle");
        }
    }
    return topological_ordering;
}
