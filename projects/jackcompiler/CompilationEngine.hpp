#ifndef COMPILATIONENGINE_H_
#define COMPILATIONENGINE_H_

#include <iostream>
#include <filesystem>
#include <fstream>
#include <set>
#include <string>

#include "Utils.hpp"
#include "JackTokenizer.hpp"

class CompilationEngine {
    std::ofstream ofs;
    JackTokenizer tokenizer;

    void compileClass();
    void compileClassVarDec();
    void compileSubroutine();
    void compileParameterList();
    void compileVarDec();
    void compileStatements();
    void compileDo();
    void compileLet();
    void compileWhile();
    void compileReturn();
    void compileIf();
    void compileExpression();
    void compileTerm();
    void compileExpressionList();

    void compileTerminal();
    void compileSubroutineBody();
    void compileStatement();
    void compileSubroutineCall();

    bool isType();
    bool isOp();
    bool isUnaryOp();
    bool isStatement();

public:
    CompilationEngine(std::filesystem::path inputFile);
    void compile();
};

#endif  // COMPILATIONENGINE_H_
