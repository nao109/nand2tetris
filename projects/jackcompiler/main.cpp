#include <iostream>
#include <filesystem>

#include "JackCompiler.hpp"

int main(int argc, char *argv[]) {
    if(argc != 2){
        std::cerr << "Cannot start program\n";
        return 1;
    }

    std::filesystem::path inputFile = argv[1];

    if(!std::filesystem::exists(inputFile)){
        std::cerr << "File does not exist\n";
        return 1;
    }

    JackCompiler compiler(inputFile);
    compiler.compile();

    return 0;
}
