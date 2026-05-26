// Runs SA with given parameters for n_trials independent runs.
// Usage: ./main2 <tsp_file> <init_temp> <cooling_rate> <iters_per_temp> <n_trials> <output_prefix>
// Outputs:
//   results/<prefix>_trials.csv   - per-trial cost and time
//   results/<prefix>_best_tour.csv - best tour found (for visualization)

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "tsp_sa.h"

static std::string basename_noext(const std::string& path) {
    auto s = path.rfind('/');
    std::string f = (s == std::string::npos) ? path : path.substr(s + 1);
    auto d = f.rfind('.');
    return (d == std::string::npos) ? f : f.substr(0, d);
}

int main(int argc, char* argv[]) {
    if (argc < 7) {
        std::cerr << "Usage: " << argv[0]
                  << " <tsp_file> <init_temp> <cooling_rate> <iters_per_temp> <n_trials> <output_prefix>\n";
        return 1;
    }
    std::string fpath       = argv[1];
    double init_temp        = std::stod(argv[2]);
    double cooling_rate     = std::stod(argv[3]);
    int    iters_per_temp   = std::stoi(argv[4]);
    int    n_trials         = std::stoi(argv[5]);
    std::string prefix      = argv[6];

    std::filesystem::create_directories("results");

    auto cities = read_tsp(fpath);
    if (cities.empty()) { std::cerr << "Cannot read " << fpath << "\n"; return 1; }
    int n = cities.size();

    SAParams p{init_temp, cooling_rate, iters_per_temp};
    std::mt19937 rng(std::random_device{}());

    double best_cost = 1e18;
    std::vector<int> best_tour;
    std::vector<double> costs;
    double total_ms = 0;

    std::ofstream trials("results/" + prefix + "_trials.csv");
    trials << "trial,cost,time_ms\n";

    for (int t = 0; t < n_trials; t++) {
        auto res = run_sa(cities, p, rng);
        costs.push_back(res.cost);
        total_ms += res.time_ms;
        trials << t << "," << (int)res.cost << "," << (int)res.time_ms << "\n";
        if (res.cost < best_cost) { best_cost = res.cost; best_tour = res.tour; }
        if ((t + 1) % 10 == 0)
            std::cout << "Trial " << (t+1) << "/" << n_trials
                      << "  best=" << (int)best_cost << "\n";
    }
    trials.close();

    double avg = 0;
    for (double c : costs) avg += c;
    avg /= costs.size();

    std::cout << "\n=== " << basename_noext(fpath) << " (" << n << " cities) ===\n"
              << "Best:    " << (int)best_cost << "\n"
              << "Average: " << (int)avg << "\n"
              << "Total time: " << (int)(total_ms / 1000) << "s\n";

    save_tour("results/" + prefix + "_best_tour.csv", best_tour, cities);
    std::cout << "Saved results/" << prefix << "_best_tour.csv\n";

    return 0;
}
