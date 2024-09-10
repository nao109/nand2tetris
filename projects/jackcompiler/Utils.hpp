#ifndef UTILS_H_
#define UTILS_H_

#include <optional>
#include <string>

enum class TokenType { KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST };

struct Token {
    TokenType tokenType;
    std::optional<std::string> keyword;
    std::optional<std::string> symbol;
    std::optional<std::string> identifier;
    std::optional<int> intVal;
    std::optional<std::string> stringVal;
};

enum class Kind { STATIC, FIELD, ARG, VAR, NONE };
std::string kind2string(Kind kind);

enum class Segment { CONST, ARG, LOCAL, STATIC, THIS, THAT, POINTER, TEMP };
enum class Command { NONE, ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT };
Command op2command(std::string command);
Command unaryop2command(std::string unaryOp);

#endif  // UTILS_H_
