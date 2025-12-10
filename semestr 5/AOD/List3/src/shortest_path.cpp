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

template<typename Key, typename Value>
class RadixHeap {
public:
    RadixHeap() : last(0), sz(0) {
        buckets.resize(65);
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

        if (!buckets[0].empty()) {
            auto kv = buckets[0].back();
            buckets[0].pop_back();
            --sz;
            last = kv.first;
            return kv;
        }


        std::size_t b = 1;
        while (b < buckets.size() && buckets[b].empty()) ++b;

        if (b == buckets.size()) {

            return {Key(0), Value()};
        }


        Key new_last = std::numeric_limits<Key>::max();
        for (auto &p : buckets[b]) {
            if (p.first < new_last) new_last = p.first;
        }
        last = new_last;


        std::vector<std::pair<Key, Value>> tmp;
        tmp.swap(buckets[b]);
        for (auto &p : tmp) {
            std::size_t nb = bucketIndex(p.first);
            buckets[nb].push_back(p);
        }

        if (!buckets[0].empty()) {
            auto kv = buckets[0].back();
            buckets[0].pop_back();
            --sz;
            last = kv.first;
            return kv;
        }


        return pop_min();
    }

private:
    std::vector<std::vector<std::pair<Key, Value>>> buckets;
    Key last;
    unsigned long long sz;


    std::size_t bucketIndex(Key key) const {
        Key diff = key ^ last;
        if (diff == 0) return 0;



        int leading = __builtin_clzll(static_cast<unsigned long long>(diff));
        int bitlen = 64 - leading;
        return static_cast<std::size_t>(bitlen);
    }
};




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





simResult singleDijkstra(Graph& G, int s, int g) {
    using dist_t = long long;
    const dist_t INF = std::numeric_limits<dist_t>::max();


    auto t0 = std::chrono::high_resolution_clock::now();


    std::vector<dist_t> dist(static_cast<size_t>(G.n), INF);
    if (s < 0 || s >= G.n) {
        std::cerr << "singleDijkstra: start node out of range: " << s << "\n";
        return make_default_result("Dijkstra", s, g, G.n, G.e, 0, G.c);
    }
    dist[s] = 0;

    using PQItem = std::pair<dist_t,int>;
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;
    pq.emplace(0, s);


    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d != dist[u]) continue;
        if (g != -1 && u == g) break;


        const auto &alist = G.adjacency_list[u];
        for (const auto &edge : alist) {
            int v = std::get<0>(edge);
            int w = std::get<1>(edge);
            if (w < 0) {

                continue;
            }
            if (dist[u] != INF && dist[u] + static_cast<dist_t>(w) < dist[v]) {
                dist[v] = dist[u] + static_cast<dist_t>(w);
                pq.emplace(dist[v], v);
            }
        }
    }


    auto t1 = std::chrono::high_resolution_clock::now();


    int minW = std::numeric_limits<int>::max();
    bool foundEdge = false;
    for (int u = 0; u < G.n; ++u) {
        for (const auto &edge : G.adjacency_list[u]) {
            int w = std::get<1>(edge);
            if (!foundEdge || w < minW) minW = w, foundEdge = true;
        }
    }
    if (!foundEdge) minW = 0;


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

simResult singleDial(Graph& G, int s, int g) {
    using dist_t = long long;
    const dist_t INF = std::numeric_limits<dist_t>::max();


    if (s < 0 || s >= G.n) {
        std::cerr << "singleDial: start node out of range: " << s << "\n";
        return make_default_result("Dial", s, g, G.n, G.e, 0, G.c);
    }


    auto t0 = std::chrono::high_resolution_clock::now();


    int C = G.c;
    int bucketCount = (C >= 0) ? (C + 1) : 1;
    if (bucketCount <= 0) bucketCount = 1;


    std::vector<dist_t> dist(static_cast<size_t>(G.n), INF);
    dist[s] = 0;


    std::vector<std::unordered_set<int>> buckets(static_cast<size_t>(bucketCount));
    buckets[0].insert(s);

    int left = 1;
    dist_t d = 0;
    int currentBucket = 0;

    while (left > 0) {

        if (buckets[static_cast<size_t>(currentBucket)].empty()) {
            currentBucket = (currentBucket + 1) % bucketCount;
            ++d;
            continue;
        }


        auto it = buckets[static_cast<size_t>(currentBucket)].begin();
        int u = *it;
        buckets[static_cast<size_t>(currentBucket)].erase(it);
        --left;



        if (dist[u] < d) {

            continue;
        }
        if (dist[u] > d) {


            int idx = static_cast<int>((currentBucket + (dist[u] - d)) % bucketCount);

            if (idx != currentBucket) {
                auto inserted = buckets[static_cast<size_t>(idx)].insert(u);
                if (inserted.second) ++left;
            }
            continue;
        }


        if (g != -1 && u == g) {
            break;
        }


        for (const auto &edge : G.adjacency_list[u]) {
            int v = std::get<0>(edge);
            int w = std::get<1>(edge);
            if (w < 0) {

                continue;
            }

            if (dist[u] != INF) {
                dist_t nd = dist[u] + static_cast<dist_t>(w);
                if (nd < dist[v]) {

                    if (dist[v] != INF) {
                        int oldIdx = static_cast<int>((currentBucket + (dist[v] - d)) % bucketCount);

                        auto erased = buckets[static_cast<size_t>(oldIdx)].erase(v);
                        if (erased > 0) --left;
                    }

                    dist[v] = nd;
                    int newIdx = static_cast<int>((currentBucket + (nd - d)) % bucketCount);
                    buckets[static_cast<size_t>(newIdx)].insert(v);
                    ++left;
                }
            }
        }
    }


    auto t1 = std::chrono::high_resolution_clock::now();


    int minW = std::numeric_limits<int>::max();
    bool foundEdge = false;
    for (int u = 0; u < G.n; ++u) {
        for (const auto &edge : G.adjacency_list[u]) {
            int w = std::get<1>(edge);
            if (!foundEdge || w < minW) minW = w, foundEdge = true;
        }
    }
    if (!foundEdge) minW = 0;


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
        if (g != -1 && u == g) break;


        for (const auto &edge : G.adjacency_list[u]) {
            int v = std::get<0>(edge);
            int w = std::get<1>(edge);
            if (w < 0) continue;


            if (dist[u] == INF) continue;
            ull nd = dist[u] + static_cast<ull>(w);
            if (nd < dist[static_cast<size_t>(v)]) {
                dist[static_cast<size_t>(v)] = nd;
                heap.push(nd, v);
            }
        }
    }

    auto t1 = std::chrono::high_resolution_clock::now();


    int minW = std::numeric_limits<int>::max();
    bool foundEdge = false;
    for (int u = 0; u < G.n; ++u) {
        for (const auto &edge : G.adjacency_list[u]) {
            int w = std::get<1>(edge);
            if (!foundEdge || w < minW) minW = w, foundEdge = true;
        }
    }
    if (!foundEdge) minW = 0;


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


        simResult r = algorithm(G, s, -1);
        output.push_back(r);

        std::cout << "  ss entry " << i << ": start=" << s
                  << " time(ms)=" << r.time.count() << "\n";
    }
    return true;
}
