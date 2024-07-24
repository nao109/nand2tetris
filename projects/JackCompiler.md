```mermaid
---
title: JackCompiler
---
classDiagram
    JackAnalyzer o-- CompilationEngine
    CompilationEngine o-- JackTokenizer
    class JackAnalyzer{
        inputFile

        JackAnalyzer(inputFile)
        analyzeFile(inputFile)
        analyze()
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

        CompilationEngine(inputFile)
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
        compileTerminal()
        compileSubroutineBody()
        compileStatement()
        compileSubroutineCall()
        isType()
        isOp()
        isUnaryOp()
    }
