#include <vector>
#include <functional>
#include <stdexcept>
#include <queue>
#include <set>
#include <tuple>


std::vector<int> make_bipartite(std::vector<std::vector<int>> adjacency_list) {
    int n = adjacency_list.size();
    std::vector<int> sets(n);
    std::set<int> unvisited;
    std::queue<std::tuple<int,int>> q;

    for (int i=0; i < n; i++) {
        unvisited.insert(i);
        sets[i] = -1;
    }


    // int curr_node = 0;

    while (!unvisited.empty()) {
        int curr_node = *unvisited.begin();
        unvisited.erase(unvisited.begin());
        int curr_set = 0;
        q.push({curr_node, curr_set});

        while (!q.empty()) {
            auto [node, set] = q.front();
            q.pop();
            // assign set value and remove from unvisited
            sets[node] = set; 
            unvisited.erase(node);
            int neighbor_set = (set + 1) % 2;
            for (auto neighbor : adjacency_list[node]) {
                // if neighbor is already in a set means it's been visited already
                if (sets[neighbor] != -1) {
                    // if neighbor's set is the same as the node set it means the graph is not bipartite
                    if (sets[neighbor] != neighbor_set) {
                        throw(std::logic_error("The graph is not bipartite"));
                    }
                }
                else {
                    // neighbor hasn't been visited yet, add it to the Queue
                    q.push({neighbor, neighbor_set});
                }
            }
        }


    }
    return sets;
}
