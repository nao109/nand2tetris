#ifndef VMTRANSLATOR_H_
#define VMTRANSLATOR_H_

#include "Utils.hpp"
#include "Parser.hpp"
#include "CodeWriter.hpp"

class VMtranslator {
    fs::path inputFile;

    void translateFile(CodeWriter &codewriter, fs::path inputFile);

public:
    VMtranslator(fs::path inputFile);
    void translate();
};

#endif  // VMTRANSLATOR_H_
