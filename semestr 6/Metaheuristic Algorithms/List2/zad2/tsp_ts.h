#pragma once
#include "tsp_sa.h"  // reuses City, read_tsp, dist, tour_cost, save_tour
#include <deque>

struct TSParams {
    int tabu_size;
    int neighbor_sample;  // random neighbors evaluated per iteration
    int max_no_improve;   // stop after this many iters without new global best
};

struct TSResult { std::vector<int> tour; double cost, time_ms; };

inline TSResult run_ts(const std::vector<City>& cities, const TSParams& p, std::mt19937& rng) {
    int n = cities.size();
    auto t0 = std::chrono::steady_clock::now();

    std::vector<int> tour(n);
    std::iota(tour.begin(), tour.end(), 0);
    std::shuffle(tour.begin(), tour.end(), rng);

    double cost = tour_cost(tour, cities);
    std::vector<int> best = tour;
    double best_cost = cost;

    // Tabu list stores (a, b) with a < b — inner endpoints of reversed segment
    std::deque<std::pair<int,int>> tabu;

    auto add_tabu = [&](int a, int b) {
        if (a > b) std::swap(a, b);
        tabu.push_back({a, b});
        if ((int)tabu.size() > p.tabu_size) tabu.pop_front();
    };

    auto is_tabu = [&](int a, int b) {
        if (a > b) std::swap(a, b);
        for (const auto& e : tabu) if (e.first == a && e.second == b) return true;
        return false;
    };

    std::uniform_int_distribution<int> ri(0, n-1);
    int no_improve = 0;

    while (no_improve < p.max_no_improve) {
        double bd = 1e18;
        int bi = -1, bj = -1;

        for (int s = 0; s < p.neighbor_sample; s++) {
            int i = ri(rng), j = ri(rng);
            if (i > j) std::swap(i, j);
            if (j - i < 2) continue;
            int j1 = (j + 1) % n;
            double d = dist(cities[tour[i]],   cities[tour[j]])
                     + dist(cities[tour[i+1]], cities[tour[j1]])
                     - dist(cities[tour[i]],   cities[tour[i+1]])
                     - dist(cities[tour[j]],   cities[tour[j1]]);
            bool tb = is_tabu(tour[i+1], tour[j]);
            // Accept if not tabu, or aspiration: move yields new global best
            if ((!tb || cost + d < best_cost) && d < bd) {
                bd = d; bi = i; bj = j;
            }
        }

        if (bi == -1) { no_improve++; continue; }

        add_tabu(tour[bi+1], tour[bj]);
        std::reverse(tour.begin() + bi + 1, tour.begin() + bj + 1);
        cost += bd;

        if (cost < best_cost) {
            best_cost = cost; best = tour; no_improve = 0;
        } else {
            no_improve++;
        }
    }

    double ms = std::chrono::duration<double, std::milli>(
        std::chrono::steady_clock::now() - t0).count();
    return {best, best_cost, ms};
}
