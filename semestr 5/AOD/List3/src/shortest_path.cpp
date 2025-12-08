// s_path.cpp
#include "shortest_path.hpp"

#include <iostream>

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

simResult singleDijkstra(Graph& G, int s, int g) {
    std::cout << "singleDijkstra called (s=" << s << ", g=" << g << ")\n";
    // Return a default simResult populated with some info from G where available
    return make_default_result("Dijkstra", s, g, G.n, G.e, 0, G.c);
}

simResult singleDial(Graph& G, int s, int g) {
    std::cout << "singleDial called (s=" << s << ", g=" << g << ")\n";
    return make_default_result("Dial", s, g, G.n, G.e, 0, G.c);
}

simResult singleRadix(Graph& G, int s, int g) {
    std::cout << "singleRadix called (s=" << s << ", g=" << g << ")\n";
    return make_default_result("RadixHeap", s, g, G.n, G.e, 0, G.c);
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

bool p2p_simulation(simResult (*algorithm)(Graph& G, int s, int g),Graph& G,
                    const std::vector<std::tuple<int, int>>& pairs,
                    std::vector<simResult>& output)
{
    std::cout << "p2p_simulation called (dummy)\n";
    output.clear();
    for (size_t i = 0; i < pairs.size(); ++i) {
        int u = std::get<0>(pairs[i]);
        int v = std::get<1>(pairs[i]);
        // produce a dummy result; no Graph available here in signature, so set v/e=0
        simResult r = make_default_result("p2p_dummy", u, v, 0, 0, 0, 0);
        output.push_back(r);
        std::cout << "  p2p entry " << i << ": (" << u << "," << v << ")\n";
    }
    return true;
}

bool ss_simulation(simResult (*algorithm)(Graph& G, int s, int g),Graph& G,
                   const std::vector<int>& start_points,
                   std::vector<simResult>& output)
{
    std::cout << "ss_simulation called (dummy)\n";
    output.clear();
    for (size_t i = 0; i < start_points.size(); ++i) {
        int s = start_points[i];
        simResult r = make_default_result("ss_dummy", s, -1, 0, 0, 0, 0);
        output.push_back(r);
        std::cout << "  ss entry " << i << ": start=" << s << "\n";
    }
    return true;
}
