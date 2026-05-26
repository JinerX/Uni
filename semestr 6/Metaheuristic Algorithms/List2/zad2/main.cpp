// Grid search for Tabu Search parameters on small TSP instances (< 1000 cities).
// Outputs:
//   results/grid_search_ts.csv  - all run stats
//   results/best_params_ts.txt  - ranked parameter combos
//   results/solution_<name>.csv - best tour per file (with best params)

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <algorithm>
#include <filesystem>
#include "tsp_ts.h"

using namespace std;

static string basename_noext(const string& path) {
    auto s = path.rfind('/');
    string f = (s == string::npos) ? path : path.substr(s + 1);
    auto d = f.rfind('.');
    return (d == string::npos) ? f : f.substr(0, d);
}

int main() {
    filesystem::create_directories("results");

    // Small files only (< 1000 cities)
    vector<string> files = {
        "../data/wi29.tsp", "../data/dj38.tsp", "../data/qa194.tsp",
        "../data/uy734.tsp", "../data/zi929.tsp"
    };

    // --- Parameter grid ---
    vector<int> tabu_sizes  = {10, 50, 100};
    vector<int> samples     = {50, 200, 500};
    vector<int> no_improves = {300, 1000, 3000};

    mt19937 rng(42);

    ofstream csv("results/grid_search_ts.csv");
    csv << "file,n_cities,tabu_size,neighbor_sample,max_no_improve,best_cost,time_ms\n";

    using Key = tuple<int, int, int>;
    map<Key, double> score_sum;
    map<string, double> best_per_file;

    struct Row { string fname; int ts, ns, ni; double cost; };
    vector<Row> rows;

    for (const auto& fpath : files) {
        auto cities = read_tsp(fpath);
        if (cities.empty()) { cerr << "Cannot read " << fpath << "\n"; continue; }
        int n = cities.size();
        string fname = basename_noext(fpath);
        cout << "\n[" << fname << "] " << n << " cities\n";

        for (int ts : tabu_sizes) {
            for (int ns : samples) {
                for (int ni : no_improves) {
                    TSParams p{ts, ns, ni};
                    auto res = run_ts(cities, p, rng);

                    csv << fname << "," << n << "," << ts << "," << ns << ","
                        << ni << "," << (int)res.cost << "," << (int)res.time_ms << "\n";

                    rows.push_back({fname, ts, ns, ni, res.cost});

                    if (!best_per_file.count(fname) || res.cost < best_per_file[fname])
                        best_per_file[fname] = res.cost;

                    cout << "  tabu=" << ts << " sample=" << ns << " no_imp=" << ni
                         << " -> " << (int)res.cost << " (" << (int)res.time_ms << "ms)\n";
                }
            }
        }
    }
    csv.close();

    // Compute score = sum(y_i / x_i) per param combo
    for (const auto& r : rows) {
        Key k{r.ts, r.ns, r.ni};
        score_sum[k] += r.cost / best_per_file.at(r.fname);
    }

    // Sort by score
    vector<pair<double, Key>> ranked;
    for (const auto& [k, s] : score_sum) ranked.push_back({s, k});
    sort(ranked.begin(), ranked.end());

    auto [best_score, best_key] = ranked.front();
    auto [best_ts, best_ns, best_ni] = best_key;

    // Write best_params_ts.txt
    {
        ofstream bp("results/best_params_ts.txt");
        bp << "Best parameters (score = sum of cost/best_cost across files):\n\n";
        bp << "tabu_size=" << best_ts << "\n"
           << "neighbor_sample=" << best_ns << "\n"
           << "max_no_improve=" << best_ni << "\n"
           << "score=" << best_score << "\n\n"
           << "All combinations ranked:\n";
        for (const auto& [s, k] : ranked) {
            auto [ts, ns, ni] = k;
            bp << "  score=" << s << "  tabu=" << ts
               << "  sample=" << ns << "  no_imp=" << ni << "\n";
        }
    }

    cout << "\nBest: tabu=" << best_ts << " sample=" << best_ns
         << " no_imp=" << best_ni << " (score=" << best_score << ")\n";

    // Re-run best params and save tours for visualization
    cout << "\nSaving solutions with best params...\n";
    for (const auto& fpath : files) {
        auto cities = read_tsp(fpath);
        string fname = basename_noext(fpath);
        TSParams p{best_ts, best_ns, best_ni};
        auto res = run_ts(cities, p, rng);
        save_tour("results/solution_" + fname + ".csv", res.tour, cities);
        cout << "  " << fname << ": " << (int)res.cost << "\n";
    }

    return 0;
}
