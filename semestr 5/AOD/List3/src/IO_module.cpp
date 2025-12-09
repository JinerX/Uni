#include "IO_module.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <iomanip>

// Helper: trim leading spaces (we only need to detect first non-space char)
static inline std::string ltrim(const std::string &s) {
    size_t i = 0;
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    return s.substr(i);
}

bool load_gr(const std::string &path, Graph &g) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "load_gr: cannot open file: " << path << '\n';
        return false;
    }

    // Reset graph
    g.n = 0;
    g.e = 0;
    g.c = 0;
    g.adjacency_list.clear();

    std::string line;
    int declared_n = -1;
    int declared_m = -1;
    int line_no = 0;

    while (std::getline(in, line)) {
        ++line_no;
        std::string s = ltrim(line);
        if (s.empty()) continue;
        if (s[0] == 'c') {
            // comment line, skip
            continue;
        }

        std::istringstream iss(s);
        char ch;
        iss >> ch;
        if (!iss) continue;

        if (ch == 'p') {
            // Problem line. Expect: p sp <n> <m>
            std::string problem_type;
            if (!(iss >> problem_type)) {
                std::cerr << "load_gr: malformed 'p' line at " << line_no << '\n';
                return false;
            }
            if (problem_type != "sp") {
                // not the expected problem type, but we can continue if it contains counts after
                // try to read n and m anyway
            }
            int n = 0, m = 0;
            if (!(iss >> n >> m)) {
                std::cerr << "load_gr: missing n/m in 'p' line at " << line_no << '\n';
                return false;
            }
            declared_n = n;
            declared_m = m;
            if (declared_n < 0) {
                std::cerr << "load_gr: invalid vertex count at line " << line_no << '\n';
                return false;
            }
            // initialize graph container sizes
            g.n = declared_n;
            g.adjacency_list.assign(g.n, std::vector<std::tuple<int,int>>());
            // don't set e yet; we'll count 'a' lines
            g.e = 0;
            g.c = 0;
        } else if (ch == 'a') {
            // Arc line. Expect: a u v w
            int u, v;
            long long w_ll; // parse as long long in case weights large
            if (!(iss >> u >> v >> w_ll)) {
                std::cerr << "load_gr: malformed 'a' line at " << line_no << ": " << s << '\n';
                return false;
            }
            if (u <= 0 || v <= 0) {
                std::cerr << "load_gr: vertex indices must be >= 1 at line " << line_no << '\n';
                return false;
            }
            if (g.n == 0) {
                // If file omitted a 'p' line, we need to grow dynamically.
                // This is a fallback: enlarge adjacency_list to accommodate max index seen.
                int needed = std::max(u, v);
                g.adjacency_list.resize(needed);
                g.n = needed;
            } else {
                // validate indices within declared range
                if (u > g.n || v > g.n) {
                    std::cerr << "load_gr: vertex index out of range at line " << line_no
                              << " (u=" << u << ", v=" << v << ", n=" << g.n << ")\n";
                    return false;
                }
            }

            // convert to 0-based
            int u0 = u - 1;
            int v0 = v - 1;
            int w = static_cast<int>(w_ll);
            if (w_ll < 0) {
                std::cerr << "load_gr: negative weight at line " << line_no << '\n';
                return false;
            }

            g.adjacency_list[u0].push_back(std::make_tuple(v0, w));
            ++g.e;
            if (w > g.c) g.c = w;
        } else {
            // unknown token: ignore but be permissive
            // (some .gr files might contain other lines; we choose to skip them)
            continue;
        }
    } // end read lines

    // If the 'p' line declared counts, verify consistency (best-effort)
    if (declared_n > 0 && g.n != declared_n) {
        // This can happen if we dynamically resized earlier; prefer declared value
        // If g.adjacency_list size was smaller, enlarge (shouldn't happen normally).
        g.adjacency_list.resize(declared_n);
        g.n = declared_n;
    }

    // If declared_m is present we don't strictly enforce it, but can warn if mismatch:
    if (declared_m >= 0 && declared_m != g.e) {
        // not an error, just inform (commented out to avoid noisy output in normal runs)
        // std::cerr << "load_gr: warning: declared m=" << declared_m << " but parsed edges=" << g.e << '\n';
    }

    return true;
}

bool load_ss(const std::string &path, std::vector<int> &sources) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "load_ss: cannot open file: " << path << '\n';
        return false;
    }

    sources.clear();
    std::string line;
    int declared_count = -1;
    int line_no = 0;

    while (std::getline(in, line)) {
        ++line_no;
        std::string s = ltrim(line);
        if (s.empty()) continue;
        if (s[0] == 'c') {
            // comment
            continue;
        }

        std::istringstream iss(s);
        char ch;
        iss >> ch;
        if (!iss) continue;

        if (ch == 'p') {
            // possible header line: p aux sp ss <count>
            // we'll try to read tokens and possibly the count at the end
            std::string t1, t2, t3;
            if ((iss >> t1 >> t2 >> t3)) {
                // last token may be count
                int maybe_count;
                if (iss >> maybe_count) {
                    declared_count = maybe_count;
                }
            }
            // continue reading other lines
        } else if (ch == 's') {
            int u;
            if (!(iss >> u)) {
                std::cerr << "load_ss: malformed 's' line at " << line_no << ": " << s << '\n';
                return false;
            }
            if (u <= 0) {
                std::cerr << "load_ss: source index must be >= 1 at line " << line_no << '\n';
                return false;
            }
            sources.push_back(u - 1); // convert to 0-based
        } else {
            // ignore unknown tokens
            continue;
        }
    }

    if (declared_count >= 0 && declared_count != static_cast<int>(sources.size())) {
        // not fatal, but inform user
        std::cerr << "load_ss: warning: declared source count = " << declared_count
                  << " but parsed = " << sources.size() << '\n';
    }

    return true;
}

bool load_p2p(const std::string &path, std::vector<std::tuple<int,int>> &pairs) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "load_p2p: cannot open file: " << path << '\n';
        return false;
    }

    pairs.clear();
    std::string line;
    int declared_count = -1;
    int line_no = 0;

    while (std::getline(in, line)) {
        ++line_no;
        std::string s = ltrim(line);
        if (s.empty()) continue;
        if (s[0] == 'c') {
            // comment
            continue;
        }

        std::istringstream iss(s);
        char ch;
        iss >> ch;
        if (!iss) continue;

        if (ch == 'p') {
            // possible header line: p aux sp p2p <count>
            // try to extract a trailing count if present
            std::string t1, t2, t3;
            if ((iss >> t1 >> t2 >> t3)) {
                int maybe_count;
                if (iss >> maybe_count) {
                    declared_count = maybe_count;
                }
            }
            // continue
        } else if (ch == 'q') {
            int u, v;
            if (!(iss >> u >> v)) {
                std::cerr << "load_p2p: malformed 'q' line at " << line_no << ": " << s << '\n';
                return false;
            }
            if (u <= 0 || v <= 0) {
                std::cerr << "load_p2p: vertex indices must be >= 1 at line " << line_no << '\n';
                return false;
            }
            pairs.emplace_back(u - 1, v - 1); // convert to 0-based
        } else {
            // ignore unknown tokens
            continue;
        }
    }

    if (declared_count >= 0 && declared_count != static_cast<int>(pairs.size())) {
        std::cerr << "load_p2p: warning: declared pair count = " << declared_count
                  << " but parsed = " << pairs.size() << '\n';
    }

    return true;
}

bool save_ss(const std::string &path,
             std::vector<simResult>& simulation_results,
             const std::string& gr_file,
             const std::string& ss_file)
{
    if (simulation_results.empty()) {
        std::cerr << "save_ss: no simulation results to save\n";
        return false;
    }

    std::ofstream out(path);
    if (!out) {
        std::cerr << "save_ss: cannot open output file: " << path << "\n";
        return false;
    }

    // Representative metadata from first result
    const simResult &r0 = simulation_results.front();
    std::string alg = r0.type.empty() ? "unknown" : r0.type;

    // Header (matches example structure)
    out << "c plik wynikowy dla problemu\n";
    out << "c najkrotszych sciezek z jednym zrodlem.\n";
    out << "c\n";
    out << "p res sp ss " << alg << "\n";
    out << "c --------------------------------------------------------------\n";
    out << "c\n";

    // 'f' line with original filenames
    out << "c wyniki testu dla sieci zadanej w pliku " << gr_file << "\n";
    out << "c i zrodel " << ss_file << ":\n";
    out << "f " << gr_file << " " << ss_file << "\n";
    out << "c\n";

    // Graph summary line: g <n> <m> <minCost> <maxCost>
    out << "c siec sklada sie z " << r0.v << " wierzcholkow, " << r0.e << " lukow,\n";
    out << "c koszty naleza do przedzialu [" << r0.minCost << "," << r0.maxCost << "]:\n";
    out << "g " << r0.v << " " << r0.e << " " << r0.minCost << " " << r0.maxCost << "\n";
    out << "c\n";

    // Average time (in milliseconds) across all simulation_results
    double sum_ms = 0.0;
    for (const auto &r : simulation_results) {
        sum_ms += static_cast<double>(r.time.count());
    }
    double avg_ms = sum_ms / static_cast<double>(simulation_results.size());

    out << "c\n";
    out << "c sredni czas wyznaczenia najkrotszych sciezek miedzy zrodlem\n";
    out << "c a wszystkimi wierzcholkami wynosi " << std::fixed << std::setprecision(2) << avg_ms << " msec:\n";
    out << "t " << std::fixed << std::setprecision(2) << avg_ms << "\n";

    out.close();
    return true;
}


bool save_p2p(const std::string &path,
              std::vector<simResult>& simulation_results,
              const std::string& gr_file,
              const std::string& p2p_file)
{
    if (simulation_results.empty()) {
        std::cerr << "save_p2p: no simulation results to save\n";
        return false;
    }

    std::ofstream out(path);
    if (!out) {
        std::cerr << "save_p2p: cannot open output file: " << path << "\n";
        return false;
    }

    // Representative metadata from first result
    const simResult &r0 = simulation_results.front();
    std::string alg = r0.type.empty() ? "unknown" : r0.type;

    // Header (matches example structure)
    out << "c plik wynikowy dla problemu\n";
    out << "c najkrotszej sciezki miedzy para wierzcholkow.\n";
    out << "c\n";
    out << "c wyniki testu dla sieci zadanej w pliku " << gr_file << "\n";
    out << "c i par zrodlo-ujscie podanych w pliku " << p2p_file << ":\n";
    out << "f " << gr_file << " " << p2p_file << "\n";
    out << "c\n";

    out << "c siec sklada sie z " << r0.v << " wierzcholkow, " << r0.e << " lukow,\n";
    out << "c koszty naleza do przedzialu [" << r0.minCost << "," << r0.maxCost << "]:\n";
    out << "g " << r0.v << " " << r0.e << " " << r0.minCost << " " << r0.maxCost << "\n";
    out << "c\n";
    out << "c dlugosci najkrotszych sciezek\n";
    out << "c (format: d <from> <to> <length>)\n";

    // Write each pair result. Convert node indices from 0-based (internal) to 1-based (output)
    for (const auto &r : simulation_results) {
        int from = r.start;
        int to   = r.end;
        int length = r.length; // -1 indicates no path / unreachable

        // convert to 1-based for output
        int out_from = (from >= 0) ? (from + 1) : -1;
        int out_to   = (to   >= 0) ? (to   + 1) : -1;

        out << "d " << out_from << " " << out_to << " " << length << "\n";
    }

    out.close();
    return true;
}