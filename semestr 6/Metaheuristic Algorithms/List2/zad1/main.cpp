// Grid search for SA parameters on small TSP instances (< 1000 cities).
// Outputs:
//   results/grid_search.csv   - all run stats
//   results/best_params.txt   - ranked parameter combos
//   results/solution_<name>.csv - best tour per file (with best params)

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <algorithm>
#include <filesystem>
#include "tsp_sa.h"

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
    vector<double> temps  = {500.0, 5000.0, 50000.0};
    vector<double> alphas = {0.90, 0.95, 0.99};
    vector<int>    iters  = {200, 1000, 5000};

    mt19937 rng(42);

    ofstream csv("results/grid_search.csv");
    csv << "file,n_cities,init_temp,cooling_rate,iters_per_temp,best_cost,time_ms\n";

    using Key = tuple<double, double, int>;
    map<Key, double> score_sum;
    map<string, double> best_per_file;

    struct Row { string fname; double T, alpha; int it; double cost; };
    vector<Row> rows;

    for (const auto& fpath : files) {
        auto cities = read_tsp(fpath);
        if (cities.empty()) { cerr << "Cannot read " << fpath << "\n"; continue; }
        int n = cities.size();
        string fname = basename_noext(fpath);
        cout << "\n[" << fname << "] " << n << " cities\n";

        for (double T : temps) {
            for (double alpha : alphas) {
                for (int it : iters) {
                    SAParams p{T, alpha, it};
                    auto res = run_sa(cities, p, rng);

                    csv << fname << "," << n << "," << T << "," << alpha << ","
                        << it << "," << (int)res.cost << "," << (int)res.time_ms << "\n";

                    rows.push_back({fname, T, alpha, it, res.cost});

                    if (!best_per_file.count(fname) || res.cost < best_per_file[fname])
                        best_per_file[fname] = res.cost;

                    cout << "  T=" << T << " a=" << alpha << " it=" << it
                         << " -> " << (int)res.cost << " (" << (int)res.time_ms << "ms)\n";
                }
            }
        }
    }
    csv.close();

    // Compute score = sum(y_i / x_i) per param combo
    // x_i = best cost for file i across all combos, y_i = cost for this combo on file i
    for (const auto& r : rows) {
        Key k{r.T, r.alpha, r.it};
        score_sum[k] += r.cost / best_per_file.at(r.fname);
    }

    // Sort by score
    vector<pair<double, Key>> ranked;
    for (const auto& [k, s] : score_sum) ranked.push_back({s, k});
    sort(ranked.begin(), ranked.end());

    auto [best_score, best_key] = ranked.front();
    auto [best_T, best_alpha, best_it] = best_key;

    // Write best_params.txt
    {
        ofstream bp("results/best_params.txt");
        bp << "Best parameters (score = sum of cost/best_cost across files):\n\n";
        bp << "init_temp=" << best_T << "\n"
           << "cooling_rate=" << best_alpha << "\n"
           << "iters_per_temp=" << best_it << "\n"
           << "score=" << best_score << "\n\n"
           << "All combinations ranked:\n";
        for (const auto& [s, k] : ranked) {
            auto [T, alpha, it] = k;
            bp << "  score=" << s << "  T=" << T << "  alpha=" << alpha << "  iters=" << it << "\n";
        }
    }

    cout << "\nBest: T=" << best_T << " alpha=" << best_alpha
         << " iters=" << best_it << " (score=" << best_score << ")\n";

    // Re-run best params and save tours for visualization
    cout << "\nSaving solutions with best params...\n";
    for (const auto& fpath : files) {
        auto cities = read_tsp(fpath);
        string fname = basename_noext(fpath);
        SAParams p{best_T, best_alpha, best_it};
        auto res = run_sa(cities, p, rng);
        save_tour("results/solution_" + fname + ".csv", res.tour, cities);
        cout << "  " << fname << ": " << (int)res.cost << "\n";
    }

    return 0;
}
