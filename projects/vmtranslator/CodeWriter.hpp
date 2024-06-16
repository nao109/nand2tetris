#ifndef CODEWRITER_H_
#define CODEWRITER_H_

#include "Utils.hpp"

class CodeWriter {
    ofstream ofs;
    string fileName;
    static int label;

public:
    CodeWriter(fs::path file);
    void setFileName(string fileName);
    void writeArithmetic(ParseElement e);
    void writePushPop(ParseElement e);
    void close();
    string newLabel();
};

#endif  // CODEWRITER_H_
