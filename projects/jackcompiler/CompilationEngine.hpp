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
#include "VMWriter.hpp"

class CompilationEngine {
    std::ofstream ofs;
    JackTokenizer tokenizer;
    SymbolTable symbolTable;
    VMWriter vmwriter;
    std::string className;
    std::string subroutineName;
    int ifLabel;
    int whileLabel;

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
    int compileExpressionList();

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

    void initLabel();
    std::string newIfLabel();
    std::string newWhileLabel();

public:
    CompilationEngine(std::filesystem::path inputFile);
    void compile();
};

#endif  // COMPILATIONENGINE_H_
