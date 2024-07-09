#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "Utils.hpp"

struct SymbolTable {
    int next_address;

public:
    SymbolTable();
    int getNextAddress();
    void incNextAddress();
    void addEntry(string symbol, int address);
    bool contains(string symbol);
    int getAddress(string symbol);
};

#endif  // SYMBOLTABLE_H_
