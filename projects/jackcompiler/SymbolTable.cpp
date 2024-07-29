#include "SymbolTable.hpp"

SymbolTable::SymbolTable() {}

void SymbolTable::startSubroutine(){
    for(std::string symbol : symbolTable[Kind::ARG]){
        kindTable.erase(symbol);
        typeTable.erase(symbol);
    }
    symbolTable[Kind::ARG].clear();

    for(std::string symbol : symbolTable[Kind::VAR]){
        kindTable.erase(symbol);
        typeTable.erase(symbol);
    }
    symbolTable[Kind::VAR].clear();
}

void SymbolTable::define(std::string name, std::string type, Kind kind){
    symbolTable[kind].insert(name);
    kindTable[name] = std::make_pair(kind, varCount(kind));
    typeTable[name] = type;
}

int SymbolTable::varCount(Kind kind){
    return (symbolTable.count(kind) ? (int)symbolTable[kind].size() : 0);
}

Kind SymbolTable::kindOf(std::string name){
    return (kindTable.count(name) ? kindTable[name].first : Kind::NONE);
}

std::string SymbolTable::typeOf(std::string name){
    return (typeTable.count(name) ? typeTable[name] : std::string());
}

int SymbolTable::indexOf(std::string name){
    return (kindTable.count(name) ? kindTable[name].second : -1);
}
