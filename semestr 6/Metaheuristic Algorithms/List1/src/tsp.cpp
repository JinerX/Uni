#include "tsp.hpp"

#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>

std::string trim(const std::string& s) {
    const auto first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    const auto last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

bool starts_with(const std::string& s, const std::string& prefix) {
    return s.rfind(prefix, 0) == 0;
}

int euc2d_distance(double x1, double y1, double x2, double y2) {
    const double dx = x1 - x2;
    const double dy = y1 - y2;
    return static_cast<int>(std::lround(std::sqrt(dx * dx + dy * dy)));
}

TspInstance load_tsplib(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Cannot open file: " + path);
    }

    TspInstance instance;
    std::string line;
    bool in_coords = false;

    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty()) {
            continue;
        }

        if (line == "EOF") {
            break;
        }

        if (!in_coords) {
            if (starts_with(line, "NAME")) {
                const auto pos = line.find(':');
                if (pos != std::string::npos) {
                    instance.name = trim(line.substr(pos + 1));
                }
            } else if (starts_with(line, "DIMENSION")) {
                const auto pos = line.find(':');
                instance.dimension = std::stoi(trim(line.substr(pos + 1)));
                instance.coords.resize(instance.dimension);
            } else if (line == "NODE_COORD_SECTION") {
                if (instance.dimension <= 0) {
                    throw std::runtime_error("DIMENSION not before NODE_COORD_SECTION.");
                }
                in_coords = true;
            }
        } else {
            std::istringstream iss(line);
            int id;
            double x, y;
            if (!(iss >> id >> x >> y)) {
                continue;
            }
            if (id < 1 || id > instance.dimension) {
                throw std::runtime_error("Id bigger than dimension.");
            }
            instance.coords[id - 1] = {x, y};
        }
    }

    if (instance.dimension <= 0) {
        throw std::runtime_error("DIMENSION not read.");
    }

    instance.dist.assign(instance.dimension, std::vector<int>(instance.dimension, 0));
    for (int i = 0; i < instance.dimension; ++i) {
        for (int j = i + 1; j < instance.dimension; ++j) {
            const auto [x1, y1] = instance.coords[i];
            const auto [x2, y2] = instance.coords[j];
            const int d = euc2d_distance(x1, y1, x2, y2);
            instance.dist[i][j] = d;
            instance.dist[j][i] = d;
        }
    }

    return instance;
}

int tour_cost(const TspInstance& instance, const std::vector<int>& tour) {
    const int n = static_cast<int>(tour.size());
    int cost = 0;
    for (int i = 0; i < n; ++i) {
        const int a = tour[i];
        const int b = tour[(i + 1) % n];
        cost += instance.dist[a][b];
    }
    return cost;
}