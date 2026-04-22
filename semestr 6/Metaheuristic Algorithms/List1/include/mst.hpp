#ifndef MST_HPP
#define MST_HPP

#include "tsp.hpp"

#include <random>
#include <vector>

struct MstResult {
    int weight = 0;
    std::vector<std::vector<int>> tree;
};

MstResult prim_mst(const TspInstance& instance);

std::vector<int> preorder_tour_from_tree(
    const std::vector<std::vector<int>>& tree,
    int root
);

#endif