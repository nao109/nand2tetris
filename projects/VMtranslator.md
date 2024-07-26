```mermaid
---
title: VMtranslator
---
classDiagram
    VMtranslator o-- Parser
    VMtranslator o-- CodeWriter
    class VMtranslator{
        string inputFile
        VMtranslator(inputFile)
        translate()
        translateFile(codewriter, inputFile)
    }
    class Parser{
        ifs
        Parser(file)
        commandType(v)
        arg1(v)
        arg2(v)
        parse()
    }
    class CodeWriter{
        ofs
        fileName
        label
        functionName
        CodeWriter(file)
        setFileName(string fileName)
        close()
        writeCode(e)
        writeArithmetic(arg1)
        writePushPop(command, arg1, arg2)
        writeLabel(label)
        writeGoto(label)
        writeIf(label)
        writeCall(functionName, numArgs)
        writeReturn()
        writeFunction(functionName, numLocals)
        newLabel()
    }
