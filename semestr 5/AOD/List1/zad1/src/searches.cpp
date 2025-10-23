#include <vector>
#include <unordered_map>
#include <iostream>
#include "searches.hpp"
#include "tree_display.hpp"
#include <stack>
#include <queue>
#include <set>

void DFS(std::vector<std::vector<int>> adjacency_lists, int start, bool print_tree) {
    std::stack<int> s;
    std::set<int> visited;
    s.push(start);
    visited.insert(start);
    std::vector<std::vector<int>> parent_list(1+adjacency_lists.size()); // parent_list[i] - all children of the i-th node
    // std::unordered_map<int,int> parents;
    // parents[start] = -1;
    parent_list[adjacency_lists.size()].push_back(start); // last element is for storing the root
    while (!s.empty()) {
        int c = s.top();
        s.pop();
        std::cout << "Visited node: " << c << std::endl;
        for (int n : adjacency_lists[c]) {
            if (!visited.count(n)) {
                parent_list[c].push_back(n);
                s.push(n);
                visited.insert(n);
            }
        }
    }
    if (print_tree) {
        display_tree(parent_list);
    }
}

void BFS(std::vector<std::vector<int>> adjacency_lists, int start, bool print_tree) {
    std::queue<int> q;
    std::set<int> visited;
    q.push(start);
    visited.insert(start);
    std::vector<std::vector<int>> parent_list(1+adjacency_lists.size()); // parent_list[i] - all children of the i-th node
    // std::unordered_map<int,int> parents;
    // parents[start] = -1;
    parent_list[adjacency_lists.size()].push_back(start); // last element is for storing the root
    while (!q.empty()) {
        int c = q.front();
        q.pop();
        std::cout << "Visited node: " << c << std::endl;
        for (int n : adjacency_lists[c]) {
            if (!visited.count(n)) {
                parent_list[c].push_back(n);
                q.push(n);
                visited.insert(n);
            }
        }
    }
    if (print_tree) {
        display_tree(parent_list);
    }
}
