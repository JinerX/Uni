#ifndef S_PATH_HPP
#define S_PATH_HPP

#include <string>
#include <chrono>
#include "graph.hpp"
#include <vector>
#include <tuple>

struct simResult {
    std::string type; // what kind of simulation was run
    int v; // number of verticies
    int e; // number of edges
    std::chrono::milliseconds time; // time the simulation took
    int start; // start node of simulation
    int end = -1; // goal node of simulation (if -1 no goal) needed only in p2p simulation
    int length = -1; // length (weighted) of the path from start to goal needed only in p2p simulation
    int minCost; // minimal edge cost
    int maxCost; // maximal edge cost
};


simResult singleDijkstra(Graph& G, int s, int g = -1);

simResult singleDial(Graph& G, int s, int g = -1);

simResult singleRadix(Graph& G, int s, int g = -1);

simResult (*algorithmFactory(std::string type))(Graph& G, int s, int g);

bool p2p_simulation(simResult (*algorithm)(Graph& G, int s, int g), Graph& G, const std::vector<std::tuple<int, int>>& pairs, std::vector<simResult>& output);

bool ss_simulation(simResult (*algorithm)(Graph& G, int s, int g),Graph& G, const std::vector<int>& start_points, std::vector<simResult>& output);



#endif