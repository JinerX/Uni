#ifndef LOADDATA_HPP
#define LOADDATA_HPP

#include <vector>
#include <string>
#include <tuple>

std::tuple<std::vector<std::vector<int>>, bool> load_standard(int argc, char** argv);
std::tuple<std::vector<std::vector<int>>, bool> load_textfile(std::string filepath);


#endif