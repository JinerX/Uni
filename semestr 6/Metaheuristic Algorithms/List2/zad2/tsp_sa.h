#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

struct City { int id; double x, y; };

inline std::vector<City> read_tsp(const std::string& path) {
    std::ifstream f(path);
    std::vector<City> cities;
    std::string line;
    bool in_coords = false;
    while (std::getline(f, line)) {
        if (line.find("NODE_COORD_SECTION") != std::string::npos) { in_coords = true; continue; }
        if (line.find("EOF") != std::string::npos) break;
        if (in_coords) {
            std::istringstream ss(line);
            City c;
            if (ss >> c.id >> c.x >> c.y) cities.push_back(c);
        }
    }
    return cities;
}

inline double dist(const City& a, const City& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return std::sqrt(dx*dx + dy*dy);
}

inline double tour_cost(const std::vector<int>& t, const std::vector<City>& c) {
    int n = t.size();
    double cost = 0;
    for (int i = 0; i < n; i++) cost += dist(c[t[i]], c[t[(i+1)%n]]);
    return cost;
}

inline void save_tour(const std::string& path, const std::vector<int>& tour, const std::vector<City>& cities) {
    std::ofstream f(path);
    f << "order,city_id,x,y\n";
    for (int i = 0; i < (int)tour.size(); i++) {
        const City& c = cities[tour[i]];
        f << i << "," << c.id << "," << c.x << "," << c.y << "\n";
    }
}

struct SAParams { double init_temp, cooling_rate; int iters_per_temp; };
struct SAResult { std::vector<int> tour; double cost, time_ms; };

inline SAResult run_sa(const std::vector<City>& cities, const SAParams& p, std::mt19937& rng) {
    int n = cities.size();
    auto t0 = std::chrono::steady_clock::now();

    std::vector<int> tour(n);
    std::iota(tour.begin(), tour.end(), 0);
    std::shuffle(tour.begin(), tour.end(), rng);

    double cost = tour_cost(tour, cities);
    std::vector<int> best = tour;
    double best_cost = cost;

    std::uniform_int_distribution<int> ri(0, n - 1);
    std::uniform_real_distribution<double> r01(0.0, 1.0);

    double T = p.init_temp;
    // run until T drops to 1.0
    int max_ep = (p.init_temp > 1.0)
        ? (int)std::ceil(std::log(1.0 / p.init_temp) / std::log(p.cooling_rate))
        : 1;

    for (int ep = 0; ep < max_ep; ep++, T *= p.cooling_rate) {
        for (int it = 0; it < p.iters_per_temp; it++) {
            int i = ri(rng), j = ri(rng);
            if (i > j) std::swap(i, j);
            if (j - i < 2) continue;

            // 2-opt: reverse tour[i+1..j]
            // removed edges: (tour[i], tour[i+1]) and (tour[j], tour[j+1])
            // added edges:   (tour[i], tour[j])   and (tour[i+1], tour[j+1])
            int j1 = (j + 1) % n;
            double d = dist(cities[tour[i]],   cities[tour[j]])
                     + dist(cities[tour[i+1]], cities[tour[j1]])
                     - dist(cities[tour[i]],   cities[tour[i+1]])
                     - dist(cities[tour[j]],   cities[tour[j1]]);

            if (d < 0 || r01(rng) < std::exp(-d / T)) {
                std::reverse(tour.begin() + i + 1, tour.begin() + j + 1);
                cost += d;
                if (cost < best_cost) { best_cost = cost; best = tour; }
            }
        }
    }

    double ms = std::chrono::duration<double, std::milli>(
        std::chrono::steady_clock::now() - t0).count();
    return {best, best_cost, ms};
}
