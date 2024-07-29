#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <map>
#include <set>
#include <string>
#include <utility>

#include "Utils.hpp"

class SymbolTable {
    std::map<Kind, std::set<std::string>> symbolTable;
    std::map<std::string, std::pair<Kind, int>> kindTable;
    std::map<std::string, std::string> typeTable;

public:
    SymbolTable();
    void startSubroutine();
    void define(std::string name, std::string type, Kind kind);
    int varCount(Kind kind);
    Kind kindOf(std::string name);
    std::string typeOf(std::string name);
    int indexOf(std::string name);
};

#endif  // SYMBOLTABLE_H_
