#ifndef SEARCHES_HPP
#define SEARCHES_HPP

#include <vector>

void BFS(std::vector<std::vector<int>> adjacency_lists, int start = 0, bool display_tree = false);

void DFS(std::vector<std::vector<int>> adjacency_lists, int start = 0, bool display_tree = false);

#endif