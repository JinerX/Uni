#include <unordered_map>
#include "tree_display.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

void print_recursively(
    int current_node,
    std::vector<std::vector<int>> parent_vector,
    std::string prefix,
    bool is_last) {
        std::cout << prefix;
        std::cout << (is_last ? "`--" : "|--");
        std::cout << current_node + 1 << std::endl;
        if (parent_vector[current_node].size()) {
            for (int i = 0; i < parent_vector[current_node].size(); i++) {
                std::string new_prefix = (is_last ? prefix + "    " : prefix + "|   ");
                print_recursively(parent_vector[current_node][i], parent_vector, new_prefix, (i==parent_vector[current_node].size()-1));
            }
        }
    }

bool comp(int a, int b) {
    return a > b;
}

void display_tree(std::vector<std::vector<int>> parent_vector) {
    std::cout << "DISPLAYING TREE" << std::endl;
    if (parent_vector.size() == 0) {
        std::cout << "Tree is empty" << std::endl;
    }
    for (auto single_row : parent_vector) {
        std::sort(single_row.begin(), single_row.end());
    }
    int root = parent_vector[parent_vector.size()-1][0];
    std::cout << root + 1 << std::endl;
    if (parent_vector[root].size()) {
        for (int i = 0; i < parent_vector[root].size(); i++) {
            print_recursively(parent_vector[root][i], parent_vector, "", (i == parent_vector[root].size() - 1));
        }
    }
}
