#include "LibParser.h"

LibParser::LibParser(std::string libs_) {
    libs = libs_;
}
bool LibParser::add_lib(std::string name) {
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
std::string LibParser::parse_lib(std::string name) {
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
std::string LibParser::name_to_path(std::string name) {
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

std::vector<std::string> LibParser::split(const std::string& s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::string LibParser::slurp(std::ifstream& in) {
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}