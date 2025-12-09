// s_path.cpp
#include "shortest_path.hpp"

#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <functional>
#include <algorithm>
#include <limits>
#include <unordered_set>
#include <climits>
#include <utility>

// Minimal Radix Heap (Key = unsigned long long, Value = int)
template<typename Key, typename Value>
class RadixHeap {
public:
    RadixHeap() : last(0), sz(0) {
        buckets.resize(65); // supports up to 64-bit keys -> 65 buckets
    }

    bool empty() const {
        return sz == 0;
    }

    void push(Key key, Value value) {
        std::size_t b = bucketIndex(key);
        buckets[b].emplace_back(key, value);
        ++sz;
    }

    std::pair<Key, Value> pop_min() {
        // If bucket 0 has elements, return one
        if (!buckets[0].empty()) {
            auto kv = buckets[0].back();
            buckets[0].pop_back();
            --sz;
            last = kv.first;
            return kv;
        }

        // find first non-empty bucket b > 0
        std::size_t b = 1;
        while (b < buckets.size() && buckets[b].empty()) ++b;
        // If no non-empty bucket found, behavior is undefined for pop_min; assume not called then.
        if (b == buckets.size()) {
            // should not happen if user checks empty()
            return {Key(0), Value()};
        }

        // find minimal key in bucket b
        Key new_last = std::numeric_limits<Key>::max();
        for (auto &p : buckets[b]) {
            if (p.first < new_last) new_last = p.first;
        }
        last = new_last;

        // move items from bucket b into appropriate buckets based on new last
        std::vector<std::pair<Key, Value>> tmp;
        tmp.swap(buckets[b]); // take contents
        for (auto &p : tmp) {
            std::size_t nb = bucketIndex(p.first);
            buckets[nb].push_back(p);
        }
        // now bucket 0 must be non-empty (if keys are consistent), so pop again
        if (!buckets[0].empty()) {
            auto kv = buckets[0].back();
            buckets[0].pop_back();
            --sz;
            last = kv.first;
            return kv;
        }

        // fallback: if still empty, call pop_min recursively (should not generally be needed)
        return pop_min();
    }

private:
    std::vector<std::vector<std::pair<Key, Value>>> buckets;
    Key last;
    unsigned long long sz;

    // compute bucket index (bit_width of (key ^ last))
    std::size_t bucketIndex(Key key) const {
        Key diff = key ^ last;
        if (diff == 0) return 0;
        // compute number of bits required to represent diff: bit_width(diff)
        // bit_width(x) = floor(log2(x)) + 1
        // use __builtin_clzll which is defined for non-zero input
        int leading = __builtin_clzll(static_cast<unsigned long long>(diff));
        int bitlen = 64 - leading; // in [1..64]
        return static_cast<std::size_t>(bitlen);
    }
};



// Helper to build a default simResult
static simResult make_default_result(const std::string &type,
                                     int start = -1,
                                     int end = -1,
                                     int v = 0,
                                     int e = 0,
                                     int minCost = 0,
                                     int maxCost = 0)
{
    simResult r;
    r.type = type;
    r.v = v;
    r.e = e;
    r.time = std::chrono::milliseconds(0);
    r.start = start;
    r.end = end;
    r.length = -1;
    r.minCost = minCost;
    r.maxCost = maxCost;
    return r;
}

// --- Dummy algorithm wrappers ------------------------------------------------

// --- Dijkstra implementation ------------------------------------------------

simResult singleDijkstra(Graph& G, int s, int g) {
    using dist_t = long long;
    const dist_t INF = std::numeric_limits<dist_t>::max();

    // measure start
    auto t0 = std::chrono::high_resolution_clock::now();

    // distances
    std::vector<dist_t> dist(static_cast<size_t>(G.n), INF);
    if (s < 0 || s >= G.n) {
        std::cerr << "singleDijkstra: start node out of range: " << s << "\n";
        return make_default_result("Dijkstra", s, g, G.n, G.e, 0, G.c);
    }
    dist[s] = 0;

    using PQItem = std::pair<dist_t,int>; // (distance, node)
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;
    pq.emplace(0, s);

    // Run Dijkstra
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d != dist[u]) continue;
        if (g != -1 && u == g) break; // early exit for p2p

        // traverse neighbors
        const auto &alist = G.adjacency_list[u];
        for (const auto &edge : alist) {
            int v = std::get<0>(edge);
            int w = std::get<1>(edge);
            if (w < 0) {
                // our algorithms assume non-negative weights
                continue;
            }
            if (dist[u] != INF && dist[u] + static_cast<dist_t>(w) < dist[v]) {
                dist[v] = dist[u] + static_cast<dist_t>(w);
                pq.emplace(dist[v], v);
            }
        }
    }

    // measure end
    auto t1 = std::chrono::high_resolution_clock::now();

    // compute min edge weight (scan graph). If no edges, set minCost = 0
    int minW = std::numeric_limits<int>::max();
    bool foundEdge = false;
    for (int u = 0; u < G.n; ++u) {
        for (const auto &edge : G.adjacency_list[u]) {
            int w = std::get<1>(edge);
            if (!foundEdge || w < minW) minW = w, foundEdge = true;
        }
    }
    if (!foundEdge) minW = 0;

    // prepare result
    simResult res;
    res.type = "Dijkstra";
    res.v = G.n;
    res.e = G.e;
    res.time = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    res.start = s;
    res.end = g;
    res.minCost = minW;
    res.maxCost = G.c;

    if (g == -1) {
        // all-sources mode (ss): length is not applicable
        res.length = -1;
    } else {
        if (dist[g] == INF) {
            res.length = -1;
        } else {
            // clamp to int if necessary (project uses int fields)
            long long val = dist[g];
            if (val > std::numeric_limits<int>::max()) res.length = std::numeric_limits<int>::max();
            else res.length = static_cast<int>(val);
        }
    }

    return res;
}

simResult singleDial(Graph& G, int s, int g) {
    using dist_t = long long;
    const dist_t INF = std::numeric_limits<dist_t>::max();

    // basic validation
    if (s < 0 || s >= G.n) {
        std::cerr << "singleDial: start node out of range: " << s << "\n";
        return make_default_result("Dial", s, g, G.n, G.e, 0, G.c);
    }

    // timing start
    auto t0 = std::chrono::high_resolution_clock::now();

    // bucket count = max edge weight + 1 (C + 1 buckets concept)
    int C = G.c;
    int bucketCount = (C >= 0) ? (C + 1) : 1;
    if (bucketCount <= 0) bucketCount = 1;

    // distances
    std::vector<dist_t> dist(static_cast<size_t>(G.n), INF);
    dist[s] = 0;

    // buckets: each bucket stores nodes whose distance maps to that bucket index
    std::vector<std::unordered_set<int>> buckets(static_cast<size_t>(bucketCount));
    buckets[0].insert(s);

    int left = 1;               // how many items are present in all buckets
    dist_t d = 0;               // current minimum distance
    int currentBucket = 0;      // index in buckets corresponding to distance d (mod bucketCount)

    while (left > 0) {
        // advance until we find a non-empty bucket
        if (buckets[static_cast<size_t>(currentBucket)].empty()) {
            currentBucket = (currentBucket + 1) % bucketCount;
            ++d;
            continue;
        }

        // take one element from the current bucket
        auto it = buckets[static_cast<size_t>(currentBucket)].begin();
        int u = *it;
        buckets[static_cast<size_t>(currentBucket)].erase(it);
        --left;

        // if this node's recorded distance is different from current `d`, skip it
        // (this can happen because we use unordered_set and mod arithmetic)
        if (dist[u] < d) {
            // shouldn't happen, but guard
            continue;
        }
        if (dist[u] > d) {
            // It means this node's distance is greater than current d but fell into this bucket due to modulo wrap.
            // Put it into the proper bucket instead of processing now.
            int idx = static_cast<int>((currentBucket + (dist[u] - d)) % bucketCount);
            // sanity check to avoid infinite loops: only reinsert if idx != currentBucket
            if (idx != currentBucket) {
                auto inserted = buckets[static_cast<size_t>(idx)].insert(u);
                if (inserted.second) ++left;
            }
            continue;
        }

        // optional early stop if we reached target in p2p mode
        if (g != -1 && u == g) {
            break;
        }

        // relax neighbors
        for (const auto &edge : G.adjacency_list[u]) {
            int v = std::get<0>(edge);
            int w = std::get<1>(edge);
            if (w < 0) {
                // Dial requires non-negative weights; skip negative edges
                continue;
            }

            if (dist[u] != INF) {
                dist_t nd = dist[u] + static_cast<dist_t>(w);
                if (nd < dist[v]) {
                    // if v was already in some bucket (finite distance), remove it
                    if (dist[v] != INF) {
                        int oldIdx = static_cast<int>((currentBucket + (dist[v] - d)) % bucketCount);
                        // safe erase, adjust left only if erased
                        auto erased = buckets[static_cast<size_t>(oldIdx)].erase(v);
                        if (erased > 0) --left;
                    }
                    // set new distance and insert into appropriate bucket
                    dist[v] = nd;
                    int newIdx = static_cast<int>((currentBucket + (nd - d)) % bucketCount);
                    buckets[static_cast<size_t>(newIdx)].insert(v);
                    ++left;
                }
            }
        }
    } // end while

    // timing end
    auto t1 = std::chrono::high_resolution_clock::now();

    // compute min edge weight (scan graph). If no edges, set minCost = 0
    int minW = std::numeric_limits<int>::max();
    bool foundEdge = false;
    for (int u = 0; u < G.n; ++u) {
        for (const auto &edge : G.adjacency_list[u]) {
            int w = std::get<1>(edge);
            if (!foundEdge || w < minW) minW = w, foundEdge = true;
        }
    }
    if (!foundEdge) minW = 0;

    // prepare result
    simResult res;
    res.type = "Dial";
    res.v = G.n;
    res.e = G.e;
    res.time = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    res.start = s;
    res.end = g;
    res.minCost = minW;
    res.maxCost = G.c;

    if (g == -1) {
        res.length = -1;
    } else {
        if (dist[g] == INF) {
            res.length = -1;
        } else {
            long long val = dist[g];
            if (val > std::numeric_limits<int>::max()) res.length = std::numeric_limits<int>::max();
            else res.length = static_cast<int>(val);
        }
    }

    return res;
}

// singleRadix using the RadixHeap
simResult singleRadix(Graph& G, int s, int g) {
    using ull = unsigned long long;
    const ull INF = std::numeric_limits<ull>::max();

    if (s < 0 || s >= G.n) {
        std::cerr << "singleRadix: start node out of range: " << s << "\n";
        return make_default_result("RadixHeap", s, g, G.n, G.e, 0, G.c);
    }

    auto t0 = std::chrono::high_resolution_clock::now();

    std::vector<ull> dist(static_cast<size_t>(G.n), INF);
    dist[s] = 0;

    RadixHeap<ull,int> heap;
    heap.push(0ULL, s);

    while (!heap.empty()) {
        auto kv = heap.pop_min();
        ull d = kv.first;
        int u = kv.second;

        if (d != dist[static_cast<size_t>(u)]) continue;
        if (g != -1 && u == g) break; // early exit for p2p

        // relax neighbors
        for (const auto &edge : G.adjacency_list[u]) {
            int v = std::get<0>(edge);
            int w = std::get<1>(edge);
            if (w < 0) continue; // only non-negative weights expected

            // check overflow when adding
            if (dist[u] == INF) continue;
            ull nd = dist[u] + static_cast<ull>(w);
            if (nd < dist[static_cast<size_t>(v)]) {
                dist[static_cast<size_t>(v)] = nd;
                heap.push(nd, v);
            }
        }
    }

    auto t1 = std::chrono::high_resolution_clock::now();

    // compute min edge weight (scan graph). If no edges, set minCost = 0
    int minW = std::numeric_limits<int>::max();
    bool foundEdge = false;
    for (int u = 0; u < G.n; ++u) {
        for (const auto &edge : G.adjacency_list[u]) {
            int w = std::get<1>(edge);
            if (!foundEdge || w < minW) minW = w, foundEdge = true;
        }
    }
    if (!foundEdge) minW = 0;

    // prepare result
    simResult res;
    res.type = "RadixHeap";
    res.v = G.n;
    res.e = G.e;
    res.time = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    res.start = s;
    res.end = g;
    res.minCost = minW;
    res.maxCost = G.c;

    if (g == -1) {
        res.length = -1;
    } else {
        ull val = dist[static_cast<size_t>(g)];
        if (val == INF) {
            res.length = -1;
        } else {
            if (val > static_cast<ull>(std::numeric_limits<int>::max())) res.length = std::numeric_limits<int>::max();
            else res.length = static_cast<int>(val);
        }
    }

    return res;
}
// --- Factory -----------------------------------------------------------------

// Returns pointer to selected algorithm function
simResult (*algorithmFactory(std::string type))(Graph& G, int s, int g) {
    std::string t = type;

    if (t == "dijkstra" || t == "d") {
        return &singleDijkstra;
    } else if (t == "dial") {
        return &singleDial;
    } else if (t == "radix" || t == "radixheap") {
        return &singleRadix;
    } else {
        std::cerr << "algorithmFactory: unknown type '" << type << "', defaulting to Dijkstra\n";
        return &singleDijkstra;
    }
}

// --- Simulation stubs --------------------------------------------------------

bool p2p_simulation(simResult (*algorithm)(Graph& G, int s, int g),
                    Graph& G,
                    const std::vector<std::tuple<int, int>>& pairs,
                    std::vector<simResult>& output)
{
    std::cout << "p2p_simulation running\n";
    output.clear();
    for (size_t i = 0; i < pairs.size(); ++i) {
        int u = std::get<0>(pairs[i]);
        int v = std::get<1>(pairs[i]);

        // run algorithm for this pair (from u to v)
        simResult r = algorithm(G, u, v);
        output.push_back(r);

        std::cout << "  p2p entry " << i << ": (" << u << "," << v << ") -> result.length=" << r.length
                  << " time(ms)=" << r.time.count() << "\n";
    }
    return true;
}

bool ss_simulation(simResult (*algorithm)(Graph& G, int s, int g),
                   Graph& G,
                   const std::vector<int>& start_points,
                   std::vector<simResult>& output)
{
    std::cout << "ss_simulation running\n";
    output.clear();
    for (size_t i = 0; i < start_points.size(); ++i) {
        int s = start_points[i];

        // run algorithm in "all-sources" mode (g = -1)
        simResult r = algorithm(G, s, -1);
        output.push_back(r);

        std::cout << "  ss entry " << i << ": start=" << s
                  << " time(ms)=" << r.time.count() << "\n";
    }
    return true;
}
