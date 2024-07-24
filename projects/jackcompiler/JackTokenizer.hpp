#ifndef JACKTOKENIZER_H_
#define JACKTOKENIZER_H_

#include "Utils.hpp"

class JackTokenizer {
    ifstream ifs;
    string tokenVal;
    vector<Token> tokens;
    unsigned long id;

    void removeOneLineComments();
    void removeSomeLinesComments();

    void addKeywordToken(string tokenVal);
    void addSymbolToken(string tokenVal);
    void addIdentifierToken(string tokenVal);
    void addIntConstToken(string tokenVal);
    void addStringConstToken(string tokenVal);

    void tokenize();
    void printTokens(fs::path inputFile);

public:
    JackTokenizer(fs::path inputFile);

    bool hasMoreTokens();
    void advance();

    TokenType tokenType();
    string keyword();
    char symbol();
    string identifier();
    int intVal();
    string stringVal();

    TokenType peekTokenType();
    char peekSymbol();
};

#endif  // JACKTOKENIZER_H_
