#ifndef JACKTOKENIZER_H_
#define JACKTOKENIZER_H_

#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <filesystem>

#include "Utils.hpp"

class JackTokenizer {
    std::ifstream ifs;
    std::string tokenVal;
    std::vector<Token> tokens;
    unsigned long id;

    void removeOneLineComments();
    void removeSomeLinesComments();

    void addKeywordToken(std::string tokenVal);
    void addSymbolToken(std::string tokenVal);
    void addIdentifierToken(std::string tokenVal);
    void addIntConstToken(std::string tokenVal);
    void addStringConstToken(std::string tokenVal);

    void tokenize();
    void printTokens(std::filesystem::path inputFile);

public:
    JackTokenizer(std::filesystem::path inputFile);

    bool hasMoreTokens();
    void advance();

    TokenType tokenType();
    std::string keyword();
    char symbol();
    std::string identifier();
    int intVal();
    std::string stringVal();

    TokenType peekTokenType();
    char peekSymbol();
};

#endif  // JACKTOKENIZER_H_
