#ifndef JACKANALYZER_H_
#define JACKANALYZER_H_

#include "Utils.hpp"
#include "JackTokenizer.hpp"
#include "CompilationEngine.hpp"

class JackAnalyzer {
    fs::path inputFile;

    void analyzeFile(fs::path inputFile);

public:
    JackAnalyzer(fs::path inputFile);
    void analyze();
};

#endif  // JACKANALYZER_H_
