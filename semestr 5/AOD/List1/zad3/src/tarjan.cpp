#include <set>
#include <vector>
#include <stack>
#include <functional>
#include <algorithm>
#include <iostream>

int UNVISITED = -1;


// Absolutely horrible readability should put it all into a struct
// void dfs(int at, std::stack<int>* stack, int ids[], int low[], bool on_stack[], std::vector<std::vector<int>>* adjacency_list, int* id, int* sccCount, std::set<std::set<int>>* SCCs) {
    // // std::cout << "Starting dfs" << std::endl;
    
    // (*stack).push(at);
    // on_stack[at] = true;
    // (*id) += 1;
    // ids[at] = (*id);
    // low[at] = (*id);

    // for (auto neighbor : (*adjacency_list)[at]) {
    //     if (ids[neighbor] == UNVISITED) {
    //         dfs(neighbor, stack, ids, low, on_stack, adjacency_list, id, sccCount, SCCs);
    //     }
    //     if (on_stack[neighbor]) {
    //         low[at] = std::min(low[at], low[neighbor]);
    //     }
    // }
    // if (ids[at] == low[at]) {
    //     std::set<int> new_SCC;
    //     while (true) {
    //         int node = (*stack).top();
    //         new_SCC.insert(node);
    //         (*stack).pop();
    //         on_stack[node] = false;
    //         low[node] = ids[at];
    //         if (node == at) {
    //             break;
    //         }
    //     }
    //     (*SCCs).insert(new_SCC);
    // }
    // (*sccCount)++;
// }

std::set<std::set<int>> tarjan(std::vector<std::vector<int>> adjacency_list) {
    int n = adjacency_list.size();

    int id = 0;
    int sccCount = 0;

    std::set<std::set<int>> SCCs;
    int ids[n];
    int low[n];
    bool on_stack[n];
    std::stack<int> stack;
    // initialize values
    for (int i = 0; i < n; i++) {
        ids[i] = 0;
        low[i] = 0;
        on_stack[i] = false;
    }
    std::function<void(int)> dfs = [&](int at) {
        // std::cout << "Starting dfs" << std::endl;
        
        stack.push(at);
        on_stack[at] = true;
        id += 1;
        ids[at] = id;
        low[at] = id;

        for (auto neighbor : adjacency_list[at]) {
            if (ids[neighbor] == UNVISITED) {
                dfs(neighbor);
            }
            if (on_stack[neighbor]) {
                low[at] = std::min(low[at], low[neighbor]);
            }
        }
        if (ids[at] == low[at]) {
            std::set<int> new_SCC;
            while (true) {
                int node = stack.top();
                new_SCC.insert(node);
                stack.pop();
                on_stack[node] = false;
                low[node] = ids[at];
                if (node == at) {
                    break;
                }
            }
            SCCs.insert(new_SCC);
        }
        sccCount++;
    };



    for (int i = 0; i < n; i++) {
        ids[i] = UNVISITED;
    }
    for (int i = 0; i < n; i++) {
        if(ids[i] == UNVISITED) {
            dfs(i);
        }
    }
    return SCCs;
}

