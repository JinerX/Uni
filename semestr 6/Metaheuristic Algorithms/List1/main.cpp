#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include <tuple>
#include <iomanip>

namespace fs = std::filesystem;


std::vector<std::string> split(const std::string& line) {
    std::stringstream ss(line);
    std::vector<std::string> words;
    std::string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}

template <typename T>
bool isNumber(const std::string& str) {
    std::stringstream ss(str);
    T value;
    ss >> value;

    return ss.eof() && !ss.fail();
}

int main() {
    std::string path = "./data";
    
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            std::cout << entry.path() << std::endl;
            std::ifstream file(entry.path());
            std::string line;
            std::vector<std::tuple<double, double>> coordinates;   
            
            while (std::getline(file, line)) {
                int idx;
                double x,y;
                std::stringstream ss(line);
                if (ss >> idx >> x >> y) {
                    coordinates.push_back({x,y});
                }
                // std::vector<std::string> split_line = split(line);
                // std::stringstream ss(line);
                // if (isNumber<int>(split_line[0])) {
                //     int idx;
                //     double x;
                //     double y;
                //     ss >> idx;
                //     ss >> x;
                //     ss >> y;
                //     coordinates.push_back({x,y});
                // }

                std::cout << line << std::endl;
            }
            int counter = 0;
            std::cout << std::fixed << std::setprecision(6);
            for (auto t : coordinates) {
                std::cout << counter << " " << std::get<0>(t) << " " << std::get<1>(t) << std::endl;
                counter++;
            }
        }
        break;
    }


    return 0;
}