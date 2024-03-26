#include <iostream>
#include "Analyzer.h"

#define TESTING_MODE

int main(int argc, char* argv[]) {
    std::string path, lib_path;
#ifndef TESTING_MODE
    if (argc == 2 || argc == 3) {
        path = argv[1];
        if (path[0] == '\"') {
            path = path.substr(1, path.size() - 2);
        }
        if (argc == 3) {
            lib_path = argv[2];
            if (lib_path[0] == '\"') {
                lib_path = lib_path.substr(1, lib_path.size() - 2);
            }
        }
    } else {
        std::cout << "Incorrect AA-arguments (2 or 3 are needed)";
    }
#endif // !TESTING_MODE
#ifdef TESTING_MODE
    path = "C:\\test.prog";
    lib_path = "E:\\Github\\AALanguage\\lib";
#endif // TESTING_MODE

    Analyzer analyzer;
    analyzer.execute(path, lib_path);
}