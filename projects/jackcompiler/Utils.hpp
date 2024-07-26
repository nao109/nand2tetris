#ifndef UTILS_H_
#define UTILS_H_

#include <optional>
#include <string>

enum class TokenType { KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST };

struct Token {
    TokenType tokenType;
    std::optional<std::string> keyword;
    std::optional<char> symbol;
    std::optional<std::string> identifier;
    std::optional<int> intVal;
    std::optional<std::string> stringVal;
};

#endif  // UTILS_H_
