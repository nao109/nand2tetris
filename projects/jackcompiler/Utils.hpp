#ifndef UTILS_H_
#define UTILS_H_

#include <bits/stdc++.h>
using namespace std;
namespace fs = filesystem;

enum class TokenType { KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST };

struct Token {
    TokenType tokenType;
    optional<string> keyword;
    optional<char> symbol;
    optional<string> identifier;
    optional<int> intVal;
    optional<string> stringVal;
};

#endif  // UTILS_H_
