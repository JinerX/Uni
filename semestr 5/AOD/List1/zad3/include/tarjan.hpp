#ifndef TARJAN_HPP
#define TARJAN_HPP

#include <set>
#include <vector>

std::set<std::set<int>> tarjan(std::vector<std::vector<int>> adjacency_list);

#endif