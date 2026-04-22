#ifndef TSP_HPP
#define TSP_HPP

#include <string>
#include <vector>

struct TspInstance {
    std::string name;
    int dimension = 0;
    std::vector<std::pair<double, double>> coords;
    std::vector<std::vector<int>> dist;
};

TspInstance load_tsplib(const std::string& path);

int tour_cost(const TspInstance& instance, const std::vector<int>& tour);

#endif