#ifndef IO_HPP
#define IO_HPP

#include <string>
#include "graph.hpp"
#include <vector>
#include <tuple>


// Load .gr file into 'g'.
// Returns true on success, false on failure (file not found or parse error).
// On success, g.n, g.e, g.c and g.adjacency_list are set.
// The function assumes directed arcs of the form: "a u v w".
bool load_gr(const std::string &path, Graph &g);

// Load .ss file (list of sources).
// On success fills 'sources' with 0-based vertex indices and returns true.
// Returns false on error (file not found or parse error).
bool load_ss(const std::string &path, std::vector<int> &sources);

// Load .p2p file (list of point-to-point queries).
// On success fills 'pairs' with 0-based (from,to) tuples and returns true.
// Returns false on error (file not found or parse error).
bool load_p2p(const std::string &path, std::vector<std::tuple<int,int>> &pairs);


#endif // IO_HPP
