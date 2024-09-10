#ifndef JACKANALYZER_H_
#define JACKANALYZER_H_

#include <filesystem>
#include <string>
#include "CompilationEngine.hpp"

class JackCompiler {
    std::filesystem::path inputFile;

    void compileFile(std::filesystem::path inputFile);

public:
    JackCompiler(std::filesystem::path inputFile);
    void compile();
};

#endif  // JACKANALYZER_H_
