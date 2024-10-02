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
        define(name, type, kind)
        varCount(kind)
        kindOf(name)
        typeOf(name)
        indexOf(name)
    }
    class VMWriter {
        ofs

        VMWriter(inputFile)
        writePush(segment, index)
        writePop(segment, index)
        writeArithmetic(command)
        writeLabel(label)
        writeGoto(label)
        writeIf(label)
        writeCall(name, nArgs)
        writeFunction(name, nLocals)
        writeReturn()
        close()
    }
