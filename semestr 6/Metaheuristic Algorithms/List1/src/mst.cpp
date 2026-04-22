#include "mst.hpp"

#include <functional>
#include <limits>
#include <stdexcept>

MstResult prim_mst(const TspInstance& instance) {
    const int n = instance.dimension;
    if (n <= 0) {
        throw std::runtime_error("Empty graph.");
    }

    std::vector<int> key(n, std::numeric_limits<int>::max());
    std::vector<int> parent(n, -1);
    std::vector<bool> in_mst(n, false);

    key[0] = 0;

    for (int iter = 0; iter < n; ++iter) {
        int u = -1;
        int best_key = std::numeric_limits<int>::max();

        for (int v = 0; v < n; ++v) {
            if (!in_mst[v] && key[v] < best_key) {
                best_key = key[v];
                u = v;
            }
        }


        in_mst[u] = true;

        for (int v = 0; v < n; ++v) {
            if (!in_mst[v] && instance.dist[u][v] < key[v]) {
                key[v] = instance.dist[u][v];
                parent[v] = u;
            }
        }
    }

    MstResult result;
    result.tree.assign(n, {});

    for (int v = 1; v < n; ++v) {
        const int p = parent[v];

        result.weight += instance.dist[v][p];
        result.tree[v].push_back(p);
        result.tree[p].push_back(v);
    }

    return result;
}

std::vector<int> preorder_tour_from_tree(
    const std::vector<std::vector<int>>& tree,
    int root
) {
    const int n = static_cast<int>(tree.size());
    if (n == 0) {
        return {};
    }

    std::vector<int> tour;
    tour.reserve(n);

    std::vector<bool> visited(n, false);

    std::function<void(int, int)> dfs = [&](int u, int parent) {
        visited[u] = true;
        tour.push_back(u);

        for (int v : tree[u]) {
            if (v != parent && !visited[v]) {
                dfs(v, u);
            }
        }
    };

    dfs(root, -1);
    return tour;
}