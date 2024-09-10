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
    std::string subroutineName;

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

    void compileSubroutineCall();

    std::string compileKeyword();
    std::string compileSymbol();
    std::string compileIdentifier();

    std::string compileClassName();
    std::string compileSubroutineName();

    void compileDec(std::string type, Kind kind);

    std::string compileType();
    Kind compileKind();

    bool consumeType(TokenType tokenType);
    bool consume(TokenType tokenType, std::string str);

    bool isType();
    bool isOp();
    bool isUnaryOp();
    bool isStatement();

public:
    CompilationEngine(std::filesystem::path inputFile);
    void compile();
};

#endif  // COMPILATIONENGINE_H_
