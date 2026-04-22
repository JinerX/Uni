#include "local_search.hpp"
#include "mst.hpp"
#include "tsp.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    try {
        const std::string input_directory = (argc >= 2) ? argv[1] : "data";
        const std::string output_path = (argc >= 3) ? argv[2] : "wyniki_task1.txt";

        std::mt19937 rng;
        if (argc >= 4) {
            rng.seed(static_cast<unsigned int>(std::stoul(argv[3])));
        } else {
            std::random_device rd;
            rng.seed(rd());
        }

        if (!std::filesystem::exists(input_directory) || !std::filesystem::is_directory(input_directory)) {
            throw std::runtime_error("Folder does not exist: " + input_directory);
        }

        std::vector<std::filesystem::path> tsp_files;
        for (const auto& entry : std::filesystem::directory_iterator(input_directory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".tsp") {
                tsp_files.push_back(entry.path());
            }
        }

        std::sort(tsp_files.begin(), tsp_files.end());

        if (tsp_files.empty()) {
            throw std::runtime_error("Folder is empty: " + input_directory);
        }

        std::ofstream out(output_path);
        if (!out) {
            throw std::runtime_error("Error opening output file" + output_path);
        }

        out << std::fixed << std::setprecision(3);

        for (const auto& file_path : tsp_files) {
            TspInstance instance = load_tsplib(file_path.string());
            const int n = instance.dimension;

            MstResult mst = prim_mst(instance);
            const int runs = static_cast<int>(std::ceil(std::sqrt(static_cast<double>(n))));

            double total_time_ms = 0.0;
            double total_steps = 0.0;
            double total_cost = 0.0;

            int best_cost = std::numeric_limits<int>::max();
            std::vector<int> best_tour;

            std::uniform_int_distribution<int> root_dist(0, n - 1);

            for (int run = 0; run < runs; ++run) {
                const int root = root_dist(rng);
                std::vector<int> start_tour = preorder_tour_from_tree(mst.tree, root);

                const auto start = std::chrono::high_resolution_clock::now();
                SearchResult result = local_search_invert_best_improvement(instance, std::move(start_tour));
                const auto end = std::chrono::high_resolution_clock::now();

                const double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();

                total_time_ms += elapsed_ms;
                total_steps += result.improvement_steps;
                total_cost += result.cost;

                if (result.cost < best_cost) {
                    best_cost = result.cost;
                    best_tour = result.tour;
                }
            }

            out << "Instance: " << instance.name << "\n";
            out << "File: " << file_path.filename().string() << "\n";
            out << "Dimension: " << n << "\n";
            out << "MST weight: " << mst.weight << "\n";
            out << "Runs: " << runs << "\n";
            out << "Average solution: " << (total_cost / runs) << "\n";
            out << "Average improvement steps: " << (total_steps / runs) << "\n";
            out << "Average time [ms]: " << (total_time_ms / runs) << "\n";
            out << "Best solution: " << best_cost << "\n";
            out << "Best tour: ";

            for (int v : best_tour) {
                out << (v + 1) << ' ';
            }
            if (!best_tour.empty()) {
                out << (best_tour.front() + 1);
            }
            out << "\n";
            out << "----------------------------------------\n";
        }

        std::cout << "Finished, files saved to: " << output_path << "\n";
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
