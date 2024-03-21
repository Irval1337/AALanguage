#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

class LibParser {
public:
    LibParser(std::string libs_) {
        libs = libs_;
    }
    bool add_lib(std::string name) {
        if (used.count(name)) return false;
        used.insert(name);

        std::string path = name_to_path(name);
        std::ifstream file;
        file.open(path, std::ios::in);
        if (file.is_open()) {
            file.close();
            return true;
        }
        throw std::exception(("Unknown library: " + name).c_str());
    }
    std::string parse_lib(std::string name) {
        std::string path = name_to_path(name);
        std::ifstream file;
        file.open(path, std::ios::in);
        if (file.is_open()) {
            std::string res = slurp(file);
            file.close();
            return res;
        }
        throw std::exception(("Unknown library: " + name).c_str());
    }
    std::string name_to_path(std::string name) {
        std::string path = libs;
        if (*path.rbegin() != '\\')
            path.push_back('\\');
        auto dirs = split(name, '.');
        for (int i = 0; i < dirs.size() - 1; ++i) {
            path += dirs[i] + "\\";
        }
        path += *dirs.rbegin() + ".aa";
        return path;
    }
private:
    std::string libs;
    std::unordered_set<std::string> used;

    std::vector<std::string> split(const std::string& s, char delim) {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> elems;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }
    std::string slurp(std::ifstream& in) {
        std::ostringstream sstr;
        sstr << in.rdbuf();
        return sstr.str();
    }
};