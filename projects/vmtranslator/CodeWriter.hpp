#ifndef CODEWRITER_H_
#define CODEWRITER_H_

#include "Utils.hpp"

class CodeWriter {
    ofstream ofs;
    string fileName;
    static int label;

    void writeArithmetic(string arg1);
    void writePushPop(string command, string arg1, int arg2);
    string newLabel();

public:
    CodeWriter(fs::path file);
    void setFileName(string fileName);
    void writeCode(ParseElement e);
    void close();
};

#endif  // CODEWRITER_H_
