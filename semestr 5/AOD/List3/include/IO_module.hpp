#ifndef IO_HPP
#define IO_HPP

#include <string>
#include "graph.hpp"
#include <vector>
#include <tuple>
#include "shortest_path.hpp"

bool load_gr(const std::string &path, Graph &g);

bool load_ss(const std::string &path, std::vector<int> &sources);

bool load_p2p(const std::string &path, std::vector<std::tuple<int,int>> &pairs);

bool save_ss(const std::string &path,
             std::vector<simResult>& simulation_results,
             const std::string& gr_file,
             const std::string& ss_file);

bool save_p2p(const std::string &path,
              std::vector<simResult>& simulation_results,
              const std::string& gr_file,
              const std::string& p2p_file);


#endif // IO_HPP
