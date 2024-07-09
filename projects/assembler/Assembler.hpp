#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include "Utils.hpp"
#include "Parser.hpp"
#include "Code.hpp"
#include "SymbolTable.hpp"

class Assembler {
    vector<string> inputFile(string input_file);
    void outputFile(string output_file, vector<string> decode);

public:
    void assemble(string input_file, string output_file);
};

#endif  // ASSEMBLER_H_
