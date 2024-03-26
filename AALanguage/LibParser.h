#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_set>

class LibParser {
public:
    LibParser(std::string libs_);
    bool add_lib(std::string name);
    std::string parse_lib(std::string name);
    std::string name_to_path(std::string name);
private:
    std::string libs;
    std::unordered_set<std::string> used;

    std::vector<std::string> split(const std::string& s, char delim);
    std::string slurp(std::ifstream& in);
};