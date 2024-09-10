#ifndef VMWRITER_H_
#define VMWRITER_H_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "Utils.hpp"

struct VMWriter {
    std::ofstream ofs;

public:
    VMWriter(std::filesystem::path inputFile);
    void writePush(Segment segment, int index);
    void writePop(Segment segment, int index);
    void writeArithmetic(Command command);
    void writeLabel(std::string label);
    void writeGoto(std::string label);
    void writeIf(std::string label);
    void writeCall(std::string name, int nArgs);
    void writeFunction(std::string name, int nLocals);
    void writeReturn();
    void close();
};

#endif  // VMWRITER_H_
