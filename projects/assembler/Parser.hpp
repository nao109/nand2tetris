#ifndef PARSER_H_
#define PARSER_H_

#include "Utils.hpp"

class Parser {
    // 今読んでいるコードのアドレス
    int address = 0;

public:
    void advance();
    int getAddress();
    string commandType(string str);
    string symbol(string str);
    string dest(string str);
    string comp(string str);
    string jump(string str);
};

#endif  // PARSER_H_
