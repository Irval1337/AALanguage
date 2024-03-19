#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_set>

class LibParser {
public:
    LibParser(std::string path_, std::string libs_) {
        path = path_;
        libs = libs_;
    }
    void execute() {
        std::ofstream out("output.aa");
        std::ifstream in(path);
        if (!in.is_open())
            throw std::exception("Incorrect file path");
        std::string add = "";
        while (!in.eof()) {
            std::string lex;
            in >> lex;
            if (lex == "using") {
                in >> lex;
                lex = parse_lib(lex);
                add += lex + " ";
            } else out << lex << " ";
        }
        out << add;
    }
    std::string parse_lib(std::string name) {
        name = name.substr(0, name.find(';'));
        if (used.count(name)) return "";

        std::string path = libs;
        if (*path.rbegin() != '\\')
            path.push_back('\\');
        auto dirs = split(name, '.');
        for (int i = 0; i < dirs.size() - 1; ++i) {
            path += dirs[i] + "\\";
        }
        path += *dirs.rbegin() + ".aa";

        std::ifstream file;
        file.open(path, std::ios::in);
        if (file.is_open()) {
            std::string res = slurp(file);
            used.insert(name);
            file.close();
            return res;
        }
        throw std::exception(("Unknown library: " + name).c_str());
    }
private:
    std::string path, libs;
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