```mermaid
---
title: JackCompiler
---
classDiagram
    JackCompiler o-- CompilationEngine
    CompilationEngine o-- JackTokenizer
    CompilationEngine o-- SymbolTable
    CompilationEngine o-- VMWriter
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
        vmwriter
        className
        subroutineName
        ifLabel
        whileLabel

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
        compileSubroutineCall()
        compileKeyword()
        compileSymbol()
        compileIdentifier()
        compileClassName()
        compileSubroutineName()
        compileDec(type, kind)
        compileType()
        compileKind()
        consumeType(tokenType)
        consume(tokenType, str)
        isType()
        isOp()
        isUnaryOp()
        isStatement()
        initLabel()
        newIfLabel()
        newWhileLabel()
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
    class VMWriter {
        ofs

        VMWriter()
        writePush()
        writePop()
        writeArithmetic()
        writeLabel()
        writeGoto()
        writeIf()
        writeCall()
        writeFunction()
        writeReturn()
        close()
    }
