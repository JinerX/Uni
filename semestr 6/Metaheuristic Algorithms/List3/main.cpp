#include <algorithm>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

namespace fs = std::filesystem;

// ─── TSP Instance ────────────────────────────────────────────────────────────

struct City { double x, y; };

class TSPInstance {
public:
    std::string name;
    std::vector<City> cities;

    bool load(const std::string& path) {
        std::ifstream f(path);
        if (!f) return false;
        std::string line;
        bool in_coords = false;
        while (std::getline(f, line)) {
            if (line.find("NAME") == 0) {
                auto pos = line.find(':');
                if (pos != std::string::npos) {
                    name = line.substr(pos + 1);
                    while (!name.empty() && (name.front() == ' ' || name.front() == '\t'))
                        name.erase(name.begin());
                    while (!name.empty() && (name.back() == ' ' || name.back() == '\r' || name.back() == '\n'))
                        name.pop_back();
                }
            } else if (line.find("NODE_COORD_SECTION") != std::string::npos) {
                in_coords = true;
            } else if (line.find("EOF") != std::string::npos) {
                break;
            } else if (in_coords) {
                std::istringstream ss(line);
                int id; double x, y;
                if (ss >> id >> x >> y)
                    cities.push_back({x, y});
            }
        }
        return !cities.empty();
    }

    int size() const { return (int)cities.size(); }

    double dist(int i, int j) const {
        double dx = cities[i].x - cities[j].x;
        double dy = cities[i].y - cities[j].y;
        return std::round(std::sqrt(dx*dx + dy*dy));
    }
};

// ─── Individual ──────────────────────────────────────────────────────────────

struct Individual {
    std::vector<int> tour;
    double fitness = 1e18;

    void evaluate(const TSPInstance& tsp) {
        int n = tsp.size();
        fitness = 0.0;
        for (int i = 0; i < n; ++i)
            fitness += tsp.dist(tour[i], tour[(i + 1) % n]);
    }

    bool operator<(const Individual& o) const { return fitness < o.fitness; }
};

// ─── Crossover Operators ─────────────────────────────────────────────────────

static Individual ox_crossover(const Individual& p1, const Individual& p2,
                                std::mt19937& rng) {
    int n = (int)p1.tour.size();
    std::uniform_int_distribution<int> dist(0, n - 1);
    int a = dist(rng), b = dist(rng);
    if (a > b) std::swap(a, b);

    Individual child;
    child.tour.assign(n, -1);
    std::vector<bool> used(n, false);

    for (int i = a; i <= b; ++i) {
        child.tour[i] = p1.tour[i];
        used[p1.tour[i]] = true;
    }

    int pos = (b + 1) % n;
    int src = (b + 1) % n;
    int filled = 0;
    while (filled < n - (b - a + 1)) {
        if (!used[p2.tour[src]]) {
            child.tour[pos] = p2.tour[src];
            pos = (pos + 1) % n;
            ++filled;
        }
        src = (src + 1) % n;
    }
    return child;
}

static Individual pmx_crossover(const Individual& p1, const Individual& p2,
                                  std::mt19937& rng) {
    int n = (int)p1.tour.size();
    std::uniform_int_distribution<int> dist(0, n - 1);
    int a = dist(rng), b = dist(rng);
    if (a > b) std::swap(a, b);

    // Position lookup tables
    std::vector<int> pos_in_p1(n), pos_in_p2(n);
    for (int i = 0; i < n; ++i) {
        pos_in_p1[p1.tour[i]] = i;
        pos_in_p2[p2.tour[i]] = i;
    }

    Individual child;
    child.tour.assign(n, -1);

    // Copy segment [a..b] from parent1
    for (int i = a; i <= b; ++i)
        child.tour[i] = p1.tour[i];

    // For each displaced element in parent2's segment, follow the mapping chain
    for (int i = a; i <= b; ++i) {
        int val = p2.tour[i];
        if (pos_in_p1[val] >= a && pos_in_p1[val] <= b) continue; // already placed
        // Follow chain: val displaced from i, p1[i] occupies i in child
        int pos = i;
        while (pos >= a && pos <= b)
            pos = pos_in_p2[p1.tour[pos]];
        child.tour[pos] = val;
    }

    // Fill remaining positions from parent2
    for (int i = 0; i < n; ++i)
        if (child.tour[i] == -1)
            child.tour[i] = p2.tour[i];

    return child;
}

// ─── GA Config ───────────────────────────────────────────────────────────────

struct GAConfig {
    int pop_size        = 100;
    int generations     = 500;
    int tournament_k    = 3;
    double xover_rate   = 0.85;
    double mut_rate     = 0.15;
    std::string xover   = "ox";   // "ox" or "pmx"
    bool memetic        = false;
    int memetic_iters   = 50;
    bool island         = false;
    int num_islands     = 4;
    int mig_interval    = 30;
    int mig_size        = 5;
    bool parallel       = false;
};

// ─── Genetic Algorithm ───────────────────────────────────────────────────────

class GeneticAlgorithm {
public:
    const TSPInstance& tsp;
    GAConfig cfg;
    std::mt19937 rng;

    std::vector<Individual> pop;
    Individual best;
    std::vector<std::pair<int,double>> conv_best;
    std::vector<std::pair<int,double>> conv_avg;

    GeneticAlgorithm(const TSPInstance& tsp_, GAConfig cfg_, unsigned seed = 42)
        : tsp(tsp_), cfg(std::move(cfg_)), rng(seed) {}

    void init() {
        int n = tsp.size();
        pop.resize(cfg.pop_size);
        std::vector<int> base(n);
        std::iota(base.begin(), base.end(), 0);
        for (auto& ind : pop) {
            ind.tour = base;
            std::shuffle(ind.tour.begin(), ind.tour.end(), rng);
            ind.evaluate(tsp);
        }
        best = *std::min_element(pop.begin(), pop.end());
    }

    Individual& tournament_select() {
        std::uniform_int_distribution<int> d(0, cfg.pop_size - 1);
        int best_idx = d(rng);
        for (int i = 1; i < cfg.tournament_k; ++i) {
            int idx = d(rng);
            if (pop[idx].fitness < pop[best_idx].fitness)
                best_idx = idx;
        }
        return pop[best_idx];
    }

    void mutate(Individual& ind) {
        int n = (int)ind.tour.size();
        std::uniform_real_distribution<double> ud(0.0, 1.0);
        if (ud(rng) > cfg.mut_rate) return;

        std::uniform_int_distribution<int> d(0, n - 1);
        int i = d(rng), j = d(rng);
        if (i > j) std::swap(i, j);
        if (i == j) return;
        // inversion mutation
        std::reverse(ind.tour.begin() + i, ind.tour.begin() + j + 1);
    }

    void apply_2opt(Individual& ind, int max_iters) {
        int n = (int)ind.tour.size();
        for (int iter = 0; iter < max_iters; ++iter) {
            bool improved = false;
            for (int i = 1; i < n - 1; ++i) {
                for (int j = i + 1; j < n; ++j) {
                    int a = ind.tour[i-1], b = ind.tour[i];
                    int c = ind.tour[j],   d = ind.tour[(j+1) % n];
                    double delta = tsp.dist(a, c) + tsp.dist(b, d)
                                 - tsp.dist(a, b) - tsp.dist(c, d);
                    if (delta < -1e-9) {
                        std::reverse(ind.tour.begin() + i,
                                     ind.tour.begin() + j + 1);
                        ind.fitness += delta;
                        improved = true;
                    }
                }
            }
            if (!improved) break;
        }
        // recompute exact fitness to avoid floating drift
        ind.evaluate(tsp);
    }

    void step_generation() {
        std::vector<Individual> next;
        next.reserve(cfg.pop_size);

        std::uniform_real_distribution<double> ud(0.0, 1.0);

        while ((int)next.size() < cfg.pop_size) {
            Individual& p1 = tournament_select();
            Individual& p2 = tournament_select();

            Individual child;
            if (ud(rng) < cfg.xover_rate) {
                if (cfg.xover == "pmx")
                    child = pmx_crossover(p1, p2, rng);
                else
                    child = ox_crossover(p1, p2, rng);
            } else {
                child = p1;
            }

            mutate(child);

            if (cfg.memetic)
                apply_2opt(child, cfg.memetic_iters);
            else
                child.evaluate(tsp);

            next.push_back(std::move(child));
        }

        // elitism: keep global best
        pop = std::move(next);
        auto it = std::min_element(pop.begin(), pop.end());
        if (it->fitness < best.fitness) best = *it;
        else pop[0] = best;
    }

    Individual run(bool save_conv = false) {
        init();
        conv_best.clear();
        conv_avg.clear();

        for (int g = 0; g < cfg.generations; ++g) {
            step_generation();

            if (save_conv) {
                double avg = 0;
                for (auto& ind : pop) avg += ind.fitness;
                avg /= cfg.pop_size;
                conv_best.push_back({g, best.fitness});
                conv_avg.push_back({g, avg});
            }
        }
        return best;
    }

    // For island model: expose population directly
    std::vector<Individual>& population() { return pop; }
    const Individual& get_best() const { return best; }
    void update_best() {
        auto it = std::min_element(pop.begin(), pop.end());
        if (it->fitness < best.fitness) best = *it;
    }
};

// ─── Island Model ────────────────────────────────────────────────────────────

class IslandModel {
    const TSPInstance& tsp;
    GAConfig cfg;
    int num_islands;
    int mig_interval;
    int mig_size;
    bool parallel;

public:
    std::vector<std::pair<int,double>> conv_best;
    std::vector<std::pair<int,double>> conv_avg;

    IslandModel(const TSPInstance& tsp_, GAConfig cfg_)
        : tsp(tsp_), cfg(cfg_),
          num_islands(cfg_.num_islands),
          mig_interval(cfg_.mig_interval),
          mig_size(cfg_.mig_size),
          parallel(cfg_.parallel) {}

    Individual run(bool save_conv = false) {
        // Create islands with different seeds
        std::vector<GeneticAlgorithm> islands;
        islands.reserve(num_islands);
        for (int i = 0; i < num_islands; ++i) {
            GAConfig island_cfg = cfg;
            island_cfg.pop_size = std::max(20, cfg.pop_size / num_islands);
            islands.emplace_back(tsp, island_cfg, 42 + i * 1000);
            islands.back().init();
        }

        Individual global_best;
        global_best.fitness = 1e18;

        conv_best.clear();
        conv_avg.clear();

        for (int g = 0; g < cfg.generations; ++g) {
            // Run one generation per island
#ifdef _OPENMP
            if (parallel) {
                #pragma omp parallel for schedule(dynamic)
                for (int i = 0; i < num_islands; ++i)
                    islands[i].step_generation();
            } else {
                for (int i = 0; i < num_islands; ++i)
                    islands[i].step_generation();
            }
#else
            for (int i = 0; i < num_islands; ++i)
                islands[i].step_generation();
#endif

            // Migration: ring topology
            if ((g + 1) % mig_interval == 0) {
                migrate(islands);
            }

            // Track global best
            for (auto& isl : islands) {
                if (isl.get_best().fitness < global_best.fitness)
                    global_best = isl.get_best();
            }

            if (save_conv) {
                double avg = 0;
                int total = 0;
                for (auto& isl : islands) {
                    for (auto& ind : isl.population()) { avg += ind.fitness; ++total; }
                }
                avg /= total;
                conv_best.push_back({g, global_best.fitness});
                conv_avg.push_back({g, avg});
            }
        }
        return global_best;
    }

private:
    void migrate(std::vector<GeneticAlgorithm>& islands) {
        int k = (int)islands.size();
        // Collect emigrants: top mig_size from each island
        std::vector<std::vector<Individual>> emigrants(k);
        for (int i = 0; i < k; ++i) {
            auto& pop = islands[i].population();
            std::vector<Individual*> sorted;
            for (auto& ind : pop) sorted.push_back(&ind);
            std::partial_sort(sorted.begin(),
                              sorted.begin() + std::min(mig_size, (int)sorted.size()),
                              sorted.end(),
                              [](const Individual* a, const Individual* b) {
                                  return a->fitness < b->fitness;
                              });
            for (int j = 0; j < std::min(mig_size, (int)sorted.size()); ++j)
                emigrants[i].push_back(*sorted[j]);
        }
        // Inject into next island (ring), replacing worst individuals
        for (int i = 0; i < k; ++i) {
            int dest = (i + 1) % k;
            auto& pop = islands[dest].population();
            // Replace worst individuals
            std::sort(pop.begin(), pop.end());
            int replace_from = (int)pop.size() - (int)emigrants[i].size();
            for (int j = 0; j < (int)emigrants[i].size(); ++j)
                pop[replace_from + j] = emigrants[i][j];
            islands[dest].update_best();
        }
    }
};

// ─── Results / CSV ───────────────────────────────────────────────────────────

struct RunResult {
    std::string instance;
    int n;
    std::string algorithm;
    std::string crossover;
    bool memetic;
    bool island;
    bool parallel;
    int run_id;
    double best_tour;
    long long time_ms;
};

static void write_summary(const std::string& path, const std::vector<RunResult>& results,
                           bool append) {
    bool write_header = !append || !fs::exists(path);
    std::ofstream f(path, append ? std::ios::app : std::ios::out);
    if (write_header)
        f << "instance,n,algorithm,crossover,memetic,island,parallel,run,best_tour,time_ms\n";
    for (auto& r : results) {
        f << r.instance << "," << r.n << "," << r.algorithm << ","
          << r.crossover << "," << (r.memetic ? "true" : "false") << ","
          << (r.island ? "true" : "false") << ","
          << (r.parallel ? "true" : "false") << ","
          << r.run_id << "," << r.best_tour << "," << r.time_ms << "\n";
    }
}

static void write_convergence(const std::string& path,
                               const std::vector<std::pair<int,double>>& best,
                               const std::vector<std::pair<int,double>>& avg) {
    std::ofstream f(path);
    f << "generation,best_fitness,avg_fitness\n";
    for (int i = 0; i < (int)best.size(); ++i)
        f << best[i].first << "," << best[i].second << "," << avg[i].second << "\n";
}

// ─── Main ─────────────────────────────────────────────────────────────────────

static void print_usage(const char* prog) {
    std::cerr << "Usage: " << prog << " <tsp_file> [options]\n"
              << "  --crossover ox|pmx    crossover method (default: ox)\n"
              << "  --pop N               population size (default: 100)\n"
              << "  --gen N               generations (default: 500)\n"
              << "  --mut-rate F          mutation rate (default: 0.15)\n"
              << "  --xover-rate F        crossover rate (default: 0.85)\n"
              << "  --tournament-k N      tournament size (default: 3)\n"
              << "  --memetic             enable memetic local search\n"
              << "  --memetic-iters N     2-opt passes per individual (default: 50)\n"
              << "  --island              enable island model\n"
              << "  --num-islands N       number of islands (default: 4)\n"
              << "  --mig-interval N      migration interval (default: 30)\n"
              << "  --mig-size N          migrants per island (default: 5)\n"
              << "  --parallel            enable OpenMP parallelization\n"
              << "  --runs N              independent runs (default: 10)\n"
              << "  --outdir PATH         output directory (default: results)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) { print_usage(argv[0]); return 1; }

    std::string tsp_file = argv[1];
    GAConfig cfg;
    int runs = 10;
    std::string outdir = "results";

    for (int i = 2; i < argc; ++i) {
        std::string a = argv[i];
        auto next = [&]() -> std::string {
            if (i + 1 < argc) return argv[++i];
            std::cerr << "Missing value for " << a << "\n"; exit(1);
        };
        if      (a == "--crossover")    cfg.xover          = next();
        else if (a == "--pop")          cfg.pop_size        = std::stoi(next());
        else if (a == "--gen")          cfg.generations     = std::stoi(next());
        else if (a == "--mut-rate")     cfg.mut_rate        = std::stod(next());
        else if (a == "--xover-rate")   cfg.xover_rate      = std::stod(next());
        else if (a == "--tournament-k") cfg.tournament_k    = std::stoi(next());
        else if (a == "--memetic")      cfg.memetic         = true;
        else if (a == "--memetic-iters")cfg.memetic_iters   = std::stoi(next());
        else if (a == "--island")       cfg.island          = true;
        else if (a == "--num-islands")  cfg.num_islands     = std::stoi(next());
        else if (a == "--mig-interval") cfg.mig_interval    = std::stoi(next());
        else if (a == "--mig-size")     cfg.mig_size        = std::stoi(next());
        else if (a == "--parallel")     cfg.parallel        = true;
        else if (a == "--runs")         runs                = std::stoi(next());
        else if (a == "--outdir")       outdir              = next();
        else { std::cerr << "Unknown option: " << a << "\n"; print_usage(argv[0]); return 1; }
    }

    TSPInstance tsp;
    if (!tsp.load(tsp_file)) {
        std::cerr << "Failed to load: " << tsp_file << "\n"; return 1;
    }
    std::cout << "Loaded " << tsp.name << " (" << tsp.size() << " cities)\n";

    fs::create_directories(outdir);

    // Build algorithm label
    std::string algo_label;
    if (cfg.island)   algo_label = "Island";
    else if (cfg.memetic) algo_label = "Memetic";
    else               algo_label = "GA";
    if (cfg.parallel)  algo_label += "_Parallel";

    std::string summary_path = outdir + "/summary.csv";
    std::vector<RunResult> results;

    for (int r = 0; r < runs; ++r) {
        unsigned seed = (unsigned)(r * 12345 + 67890);
        bool save_conv = (r == 0);

        auto t0 = std::chrono::steady_clock::now();
        Individual best;

        if (cfg.island) {
            IslandModel model(tsp, cfg);
            best = model.run(save_conv);
            if (save_conv) {
                std::string conv_path = outdir + "/" + tsp.name + "_" + algo_label + "_" + cfg.xover + "_conv.csv";
                write_convergence(conv_path, model.conv_best, model.conv_avg);
            }
        } else {
            GeneticAlgorithm ga(tsp, cfg, seed);
            best = ga.run(save_conv);
            if (save_conv) {
                std::string conv_path = outdir + "/" + tsp.name + "_" + algo_label + "_" + cfg.xover + "_conv.csv";
                write_convergence(conv_path, ga.conv_best, ga.conv_avg);
            }
        }

        auto t1 = std::chrono::steady_clock::now();
        long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

        results.push_back({
            tsp.name, tsp.size(), algo_label, cfg.xover,
            cfg.memetic, cfg.island, cfg.parallel,
            r + 1, best.fitness, ms
        });

        std::cout << "  run " << r+1 << "/" << runs
                  << "  best=" << best.fitness
                  << "  time=" << ms << "ms\n";
    }

    write_summary(summary_path, results, true);
    std::cout << "Results appended to " << summary_path << "\n";
    return 0;
}
