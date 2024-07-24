```mermaid
---
title: Assembler
---
classDiagram
    Assembler o-- Parser
    Assembler o-- Code
    Assembler o-- SymbolTable
    class Assembler{
        inputFile(input_file)
        outputFile(output_file, decode)
        assemble(input_file, output_file)
    }
    class Parser{
        address
        advance()
        getAddress()
        commandType(str)
        symbol(str)
        dest(str)
        comp(str)
        jump(str)
    }
    class Code{
        dest(dst)
        comp(cmp)
        jump(jmp)
    }
    class SymbolTable{
        next_address
        SymbolTable()
        getNextAddress()
        incNextAddress()
        addEntry(symbol, address)
        contains(symbol)
        getAddress(symbol)
    }
