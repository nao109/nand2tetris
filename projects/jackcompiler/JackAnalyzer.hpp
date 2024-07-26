#ifndef JACKANALYZER_H_
#define JACKANALYZER_H_

#include <filesystem>
#include <string>
#include "CompilationEngine.hpp"

class JackAnalyzer {
    std::filesystem::path inputFile;

    void analyzeFile(std::filesystem::path inputFile);

public:
    JackAnalyzer(std::filesystem::path inputFile);
    void analyze();
};

#endif  // JACKANALYZER_H_
