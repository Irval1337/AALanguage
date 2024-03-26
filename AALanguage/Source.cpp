#include <iostream>
#include "Analyzer.h"

//! Define used for testing on the developer's machine
#define TESTING_MODE

/**
* @brief Entry point
*
* Execution of the program starts here. 
* 
* Command line startup arguments: aalanguage.exe 'path' 'lib_path'
* - 'path' - path to the source code file
* - 'lib_path' - path to the directory with built-in libraries
* 
* @param argc Number of arguments
* @param argv List of arguments
*
* @return Program exit status
*/
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