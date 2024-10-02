```mermaid
---
title: VMtranslator
---
classDiagram
    VMtranslator o-- Parser
    VMtranslator o-- CodeWriter
    class VMtranslator{
        inputFile
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
        setFileName(fileName)
        writeCode(e)
        close()
        writeArithmetic(arg1)
        writePushPop(command, arg1, arg2)
        writeLabel(label)
        writeGoto(label)
        writeIf(label)
        writeCall(functionName, numArgs)
        writeReturn()
        writeFunction(functionName, numLocals)
        writeComments(comments)
        newLabel()
    }
