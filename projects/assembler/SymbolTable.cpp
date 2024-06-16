#include "SymbolTable.hpp"

map<string, int> symboltable = {
    {      "SP",     0},
    {     "LCL",     1},
    {     "ARG",     2},
    {    "THIS",     3},
    {    "THAT",     4},
    {      "R0",     0},
    {      "R1",     1},
    {      "R2",     2},
    {      "R3",     3},
    {      "R4",     4},
    {      "R5",     5},
    {      "R6",     6},
    {      "R7",     7},
    {      "R8",     8},
    {      "R9",     9},
    {     "R10",    10},
    {     "R11",    11},
    {     "R12",    12},
    {     "R13",    13},
    {     "R14",    14},
    {     "R15",    15},
    {  "SCREEN", 16384},
    {"KEYBOARD", 24576}
};

SymbolTable::SymbolTable(){
    // RAM[16]から追加
    next_address = 16;
}

int SymbolTable::getNextAddress(){
    return next_address;
}

void SymbolTable::incNextAddress(){
    next_address++;
}

void SymbolTable::addEntry(string symbol, int address){
    symboltable[symbol] = address;
}

bool SymbolTable::contains(string symbol){
    return symboltable.count(symbol);
}

int SymbolTable::getAddress(string symbol){
    return symboltable[symbol];
}
