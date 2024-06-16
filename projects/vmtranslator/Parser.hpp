#ifndef PARSER_H_
#define PARSER_H_

#include "VMtranslator.hpp"

class Parser {
    ifstream ifs;

public:
    Parser(fs::path file);
    string commandType(const vector<string> &v);
    string arg1(const vector<string> &v);
    int arg2(const vector<string> &v);
    vector<ParseElement> parse();
};

#endif  // PARSER_H_
