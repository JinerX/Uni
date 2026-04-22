#include "local_search.hpp"

#include <algorithm>
#include <numeric>
#include <unordered_set>

std::vector<int> random_permutation(int n, std::mt19937& rng) {
    std::vector<int> tour(n);
    std::iota(tour.begin(), tour.end(), 0);
    std::shuffle(tour.begin(), tour.end(), rng);
    return tour;
}

SearchResult local_search_invert_best_improvement(
    const TspInstance& instance,
    std::vector<int> tour) {
    const int n = static_cast<int>(tour.size());
    int current_cost = tour_cost(instance, tour);
    int steps = 0;

    while (true) {
        int best_delta = 0;
        int best_i = -1;
        int best_j = -1;

        for (int i = 0; i < n - 1; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (i == 0 && j == n - 1) {
                    continue;
                }

                const int a = tour[(i - 1 + n) % n];
                const int b = tour[i];
                const int c = tour[j];
                const int d = tour[(j + 1) % n];

                const int old_edges = instance.dist[a][b] + instance.dist[c][d];
                const int new_edges = instance.dist[a][c] + instance.dist[b][d];
                const int delta = new_edges - old_edges;

                if (delta < best_delta) {
                    best_delta = delta;
                    best_i = i;
                    best_j = j;
                }
            }
        }

        if (best_delta >= 0) {
            break;
        }

        std::reverse(tour.begin() + best_i, tour.begin() + best_j + 1);
        current_cost += best_delta;
        ++steps;
    }

    return {tour, current_cost, steps};
}

SearchResult local_search_invert_random_sample(
    const TspInstance& instance,
    std::vector<int> tour,
    std::mt19937& rng) {
    const int n = static_cast<int>(tour.size());
    int current_cost = tour_cost(instance, tour);
    int steps = 0;

    while (true) {
        int best_delta = 0;
        int best_i = -1;
        int best_j = -1;

        std::uniform_int_distribution<int> dist(0, n - 1);
        std::unordered_set<long long> used_moves;
        used_moves.reserve(static_cast<std::size_t>(n) * 2);

        int sampled = 0;
        while (sampled < n) {
            int i = dist(rng);
            int j = dist(rng);

            if (i == j) {
                continue;
            }

            if (i > j) {
                std::swap(i, j);
            }

            if (i == 0 && j == n - 1) {
                continue;
            }

            const long long key = (static_cast<long long>(i) << 32) | static_cast<unsigned int>(j);
            if (!used_moves.insert(key).second) {
                continue;
            }

            const int a = tour[(i - 1 + n) % n];
            const int b = tour[i];
            const int c = tour[j];
            const int d = tour[(j + 1) % n];

            const int old_edges = instance.dist[a][b] + instance.dist[c][d];
            const int new_edges = instance.dist[a][c] + instance.dist[b][d];
            const int delta = new_edges - old_edges;

            if (delta < best_delta) {
                best_delta = delta;
                best_i = i;
                best_j = j;
            }

            ++sampled;
        }

        if (best_delta >= 0) {
            break;
        }

        std::reverse(tour.begin() + best_i, tour.begin() + best_j + 1);
        current_cost += best_delta;
        ++steps;
    }

    return {tour, current_cost, steps};
}