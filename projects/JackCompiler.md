```mermaid
---
title: JackCompiler
---
classDiagram
    JackCompiler o-- CompilationEngine
    CompilationEngine o-- JackTokenizer
    CompilationEngine o-- SymbolTable
    class JackCompiler{
        inputFile

        JackCompiler(inputFile)
        compileFile(inputFile)
        compile()
    }
    class JackTokenizer{
        ifs
        tokenVal
        tokens
        id

        JackTokenizer(inputFile)
        hasMoreTokens()
        advance()
        tokenType()
        keyword()
        symbol()
        identifier()
        intVal()
        stringVal()
        peekTokenType()
        peekSymbol()
        printTokens(inputFile)
        removeOneLineComments()
        removeSomeLinesComments()
        tokenize()
        addKeywordToken(tokenVal)
        addSymbolToken(tokenVal)
        addIdentifierToken(tokenVal)
        addIntConstToken(tokenVal)
        addStringConstToken(tokenVal)
    }
    class CompilationEngine{
        ofs
        tokenizer
        symbolTable
        className

        CompilationEngine(inputFile)
        compile()
        compileClass()
        compileClassVarDec()
        compileSubroutine()
        compileParameterList()
        compileVarDec()
        compileStatements()
        compileDo()
        compileLet()
        compileWhile()
        compileReturn()
        compileIf()
        compileExpression()
        compileTerm()
        compileExpressionList()
        compileKeyword()
        compileSymbol()
        compileIdentifier()
        compileDec()
        compileType()
        compileKind()
        isType()
        isOp()
        isUnaryOp()
        isStatement()
    }
    class SymbolTable {
        symboltable
        kindTable
        typeTable

        SymbolTable()
        startSubroutine()
        define()
        varCount()
        kindOf()
        typeOf()
        indexOf()
    }
