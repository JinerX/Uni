// main.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <exception>
#include <cxxopts.hpp>
#include <tuple>

#include "graph.hpp"
#include "IO_module.hpp" // declares: bool load_gr(const std::string&, Graph&);
#include "shortest_path.hpp"

static bool file_exists(const std::string &path) {
    std::ifstream f(path);
    return static_cast<bool>(f);
}

static std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

static void print_sim_results(const std::vector<simResult> &results) {
    std::cout << "=== Simulation results (" << results.size() << ") ===\n";
    for (size_t i = 0; i < results.size(); ++i) {
        const simResult &r = results[i];
        std::cout << "result[" << i << "]: type=" << r.type
                  << " start=" << r.start;
        if (r.end != -1) std::cout << " end=" << r.end;
        if (r.length != -1) std::cout << " length=" << r.length;
        std::cout << " time(ms)=" << r.time.count()
                  << " v=" << r.v << " e=" << r.e
                  << " minCost=" << r.minCost << " maxCost=" << r.maxCost
                  << "\n";
    }
    std::cout << "=====================================\n";
}

int main(int argc, char* argv[]) {
    // Determine program basename
    std::string prog(argv[0]);
    auto pos = prog.find_last_of("/\\");
    if (pos != std::string::npos) prog = prog.substr(pos + 1);
    std::string prog_low = to_lower(prog);

    // Default algorithm from program name
    std::string algorithm = "dijkstra";
    if (prog_low.find("dijkstra") != std::string::npos) algorithm = "dijkstra";
    else if (prog_low.find("dial") != std::string::npos) algorithm = "dial";
    else if (prog_low.find("radix") != std::string::npos) algorithm = "radixheap";

    // If first positional arg (argv[1]) is NOT an option (doesn't start with '-'),
    // treat it as algorithm name and skip it for option parsing.
    int skip_index = -1;
    if (argc >= 2) {
        std::string first(argv[1]);
        if (!first.empty() && first[0] != '-') {
            std::string a = to_lower(first);
            if (a == "dijkstra" || a == "d") algorithm = "dijkstra";
            else if (a == "dial") algorithm = "dial";
            else if (a == "radix" || a == "radixheap") algorithm = "radixheap";
            // mark argv[1] to be skipped when building argv2
            skip_index = 1;
        }
    }

    // Normalize arguments: convert "-ss" style (single dash + multi-char) -> "--ss"
    // so cxxopts can parse multi-char option names. Skip skip_index if set.
    std::vector<std::string> storage;
    storage.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        if (i == skip_index) continue; // skip algorithm positional token
        std::string s(argv[i]);
        if (s.size() > 2 && s[0] == '-' && s[1] != '-') {
            storage.push_back("--" + s.substr(1));
        } else {
            storage.push_back(s);
        }
    }

    // Build argv array for cxxopts parse
    std::vector<char*> argv2;
    argv2.reserve(storage.size());
    for (auto &str : storage) argv2.push_back(const_cast<char*>(str.c_str()));
    int argc2 = static_cast<int>(argv2.size());

    cxxopts::Options options(prog, "CLI wrapper for shortest-path algorithms (only CLI, no loading here)");
    options.add_options()
        ("a,algo", "Algorithm name: dijkstra | dial | radixheap", cxxopts::value<std::string>())
        ("d,data", "Input graph file (.gr)", cxxopts::value<std::string>())
        ("ss",   "Sources file (for all-sources mode)", cxxopts::value<std::string>())
        ("oss",  "Output results file for all-sources mode", cxxopts::value<std::string>())
        ("p2p",  "Pairs file (point-to-point mode)", cxxopts::value<std::string>())
        ("op2p", "Output results file for point-to-point mode", cxxopts::value<std::string>())
        ("h,help", "Print help");

    cxxopts::ParseResult result;
    try {
        result = options.parse(argc2, argv2.data());
    } catch (const std::exception &e) {
        std::cerr << "Error parsing options: " << e.what() << "\n\n";
        std::cerr << options.help() << "\n";
        return 2;
    }

    // If user provided --algo, override algorithm
    if (result.count("algo")) {
        std::string a = to_lower(result["algo"].as<std::string>());
        if (a == "dijkstra" || a == "d") algorithm = "dijkstra";
        else if (a == "dial") algorithm = "dial";
        else if (a == "radix" || a == "radixheap") algorithm = "radixheap";
        else {
            std::cerr << "Unknown algorithm: " << result["algo"].as<std::string>() << "\n";
            return 11;
        }
    }

    if (result.count("help")) {
        std::cout << options.help() << "\n";
        return 0;
    }

    // Must have data file
    if (!result.count("data")) {
        std::cerr << "Error: missing required -d <data_file> option.\n";
        std::cerr << "Usage examples:\n"
                  << "  ./spa dijkstra -d graph.gr -ss sources.ss -oss out.ss.res\n"
                  << "  ./spa dial -d graph.gr -p2p pairs.p2p -op2p out.p2p.res\n"
                  << "  ./spa -a radixheap -d graph.gr -ss sources.ss -oss out.ss.res\n";
        return 3;
    }

    std::string data_file = result["data"].as<std::string>();

    // Check data file existence and load it
    if (!file_exists(data_file)) {
        std::cerr << "Error: data file does not exist: " << data_file << "\n";
        return 4;
    }

    // Load graph using IO module
    Graph g;
    if (!load_gr(data_file, g)) {
        std::cerr << "Error: failed to load graph from: " << data_file << "\n";
        return 12;
    }

    // Determine mode: either ss (all-sources) or p2p (point-to-point)
    bool has_ss  = result.count("ss");
    bool has_p2p = result.count("p2p");

    if (has_ss && has_p2p) {
        std::cerr << "Error: cannot use both -ss and -p2p modes at the same time.\n";
        return 5;
    }

    std::cout << "DEBUG: Program: " << prog << "\n";
    std::cout << "DEBUG: Selected algorithm: " << algorithm << "\n";

    // Print loaded graph summary
    std::cout << "DEBUG: Loaded graph: n=" << g.n << " e=" << g.e << " max_weight=" << g.c << "\n";

    // Print adjacency list for debugging
    // adjacency_list[u] contains tuples (to, weight), nodes stored 0-based
    for (int u = 0; u < g.n; ++u) {
        std::cout << "v[" << u << "]:";
        if (u < static_cast<int>(g.adjacency_list.size())) {
            const auto &alist = g.adjacency_list[u];
            for (const auto &t : alist) {
                int to = std::get<0>(t);
                int w  = std::get<1>(t);
                std::cout << " ->(" << to << "," << w << ")";
            }
        }
        std::cout << "\n";
    }

    auto algo_ptr = algorithmFactory(to_lower(algorithm));

    if (has_ss) {
        if (!result.count("oss")) {
            std::cerr << "Error: -ss provided but missing -oss <output_file>\n";
            return 6;
        }
        std::string ss_file  = result["ss"].as<std::string>();
        std::string oss_file = result["oss"].as<std::string>();

        if (!file_exists(ss_file)) {
            std::cerr << "Error: sources file does not exist: " << ss_file << "\n";
            return 7;
        }

        // Load sources
        std::vector<int> sources;
        if (!load_ss(ss_file, sources)) {
            std::cerr << "Error: failed to load sources from: " << ss_file << "\n";
            return 13;
        }

        // Validate sources are within graph range
        for (size_t i = 0; i < sources.size(); ++i) {
            int s = sources[i];
            if (s < 0 || s >= g.n) {
                std::cerr << "Error: source " << s << " (index " << i << ") out of range [0," << g.n-1 << "]\n";
                return 14;
            }
        }

        std::cout << "DEBUG: Mode: ALL-SOURCES (-ss)\n";
        std::cout << "DEBUG: Sources file: " << ss_file << " (exists)\n";
        std::cout << "DEBUG: Output file (results): " << oss_file << "\n";

        // Run simulation (ss)
        std::vector<simResult> results;
        bool ok = ss_simulation(algo_ptr, g, sources, results);
        if (!ok) {
            std::cerr << "Error: ss_simulation failed\n";
            return 18;
        }

        // Print results returned by simulation
        print_sim_results(results);

        if (!save_ss(oss_file, results, data_file, ss_file)) {
            std::cerr << "Error: save_ss failed for file: " << oss_file << "\n";
            return 20;
        }
        std::cout << "DEBUG: Results saved to " << oss_file << "\n";

        std::cout << "DEBUG: (placeholder) -> run algorithm '" << algorithm << "' for all sources\n";
        // Here you would eventually write results to oss_file
    } else if (has_p2p) {
        if (!result.count("op2p")) {
            std::cerr << "Error: -p2p provided but missing -op2p <output_file>\n";
            return 8;
        }
        std::string p2p_file  = result["p2p"].as<std::string>();
        std::string op2p_file = result["op2p"].as<std::string>();

        if (!file_exists(p2p_file)) {
            std::cerr << "Error: pairs file does not exist: " << p2p_file << "\n";
            return 9;
        }

        // Load pairs
        std::vector<std::tuple<int,int>> pairs;
        if (!load_p2p(p2p_file, pairs)) {
            std::cerr << "Error: failed to load pairs from: " << p2p_file << "\n";
            return 15;
        }

        // Validate pairs are within graph range
        for (size_t i = 0; i < pairs.size(); ++i) {
            int u = std::get<0>(pairs[i]);
            int v = std::get<1>(pairs[i]);
            if (u < 0 || u >= g.n) {
                std::cerr << "Error: pair[" << i << "] from=" << u << " out of range [0," << g.n-1 << "]\n";
                return 16;
            }
            if (v < 0 || v >= g.n) {
                std::cerr << "Error: pair[" << i << "] to=" << v << " out of range [0," << g.n-1 << "]\n";
                return 17;
            }
        }

        std::cout << "DEBUG: Mode: POINT-TO-POINT (-p2p)\n";
        std::cout << "DEBUG: Pairs file: " << p2p_file << " (exists)\n";
        std::cout << "DEBUG: Output file (results): " << op2p_file << "\n";

        // Run simulation (p2p)
        std::vector<simResult> results;
        bool ok = p2p_simulation(algo_ptr, g, pairs, results);
        if (!ok) {
            std::cerr << "Error: p2p_simulation failed\n";
            return 19;
        }

        // Print results returned by simulation
        print_sim_results(results);
        // Save results to output file (write f-line using data_file and p2p_file)
        if (!save_p2p(op2p_file, results, data_file, p2p_file)) {
            std::cerr << "Error: save_p2p failed for file: " << op2p_file << "\n";
            return 21;
        }
        std::cout << "DEBUG: Results saved to " << op2p_file << "\n";

        std::cout << "DEBUG: (placeholder) -> run algorithm '" << algorithm << "' for pairs\n";
        // Here you would eventually write results to op2p_file
    } else {
        std::cerr << "Error: neither -ss nor -p2p specified. One of them must be provided.\n";
        return 10;
    }

    std::cout << "DEBUG: CLI processing finished successfully.\n";
    return 0;
}
