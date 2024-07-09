#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

namespace Assembler {
    struct ParseElement {
        string commandType;
        int address;
        string symbol;
        string dest;
        string comp;
        string jump;
    };
}

#include "SymbolTable.hpp"
#include "Parser.hpp"
#include "Code.hpp"

#endif  // ASSEMBLER_H_
