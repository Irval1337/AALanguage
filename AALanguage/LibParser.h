#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_set>

//! A helper class for getting information about built-in libraries.
class LibParser {
public:
    /**
    * @brief Constructor that saves the path to the standard libraries directory.
    *
    * @param libs_ library directory path
    */
    LibParser(std::string libs_);
    /**
    * @brief Adds the new library to the list of libraries in use, if necessary.
    *
    * @param name library name with . splitting
    * 
    * @return True if the library has not yet been used. Otherwise false.
    */
    bool add_lib(std::string name);
    /**
    * @brief Reads all text from the library file.
    *
    * @param name library name with . splitting
    *
    * @return Full library file text.
    */
    std::string parse_lib(std::string name);
    /**
    * @brief Converts the standard library name to its file path.
    *
    * @param name library name with . splitting
    *
    * @return Library file path.
    */
    std::string name_to_path(std::string name);
private:
    std::string libs;
    std::unordered_set<std::string> used;

    std::vector<std::string> split(const std::string& s, char delim);
    std::string slurp(std::ifstream& in);
};