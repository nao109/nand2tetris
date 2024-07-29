#ifndef COMPILATIONENGINE_H_
#define COMPILATIONENGINE_H_

#include <iostream>
#include <filesystem>
#include <fstream>
#include <set>
#include <string>

#include "Utils.hpp"
#include "JackTokenizer.hpp"
#include "SymbolTable.hpp"

class CompilationEngine {
    std::ofstream ofs;
    JackTokenizer tokenizer;
    SymbolTable symbolTable;
    std::string className;

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

    void compileKeyword();
    void compileSymbol();
    std::string compileIdentifier();

    void compileClassName();
    void compileSubroutineName();

    void compileDec(std::string type, Kind kind);

    std::string compileType();
    Kind compileKind();

    bool isType();
    bool isOp();
    bool isUnaryOp();
    bool isStatement();

public:
    CompilationEngine(std::filesystem::path inputFile);
    void compile();
};

#endif  // COMPILATIONENGINE_H_
