#include "CodeWriter.hpp"

int CodeWriter::label = 0;

CodeWriter::CodeWriter(fs::path file) {
    string outputFile = file.string();
    if(!fs::is_directory(file)){
        outputFile.replace(outputFile.rfind(".vm"), 3, ".asm");
    }
    else{
        outputFile.append(".asm");
    }
    ofs.open(outputFile);

    this->functionName = "NULL";
}

void CodeWriter::setFileName(string fileName){
    this->fileName = fileName.substr(0, fileName.rfind(".vm"));
}

void CodeWriter::writeArithmetic(string command){
    // 単項演算子
    if(command == "neg" || command == "not"){
        ofs << "@SP\n";
        ofs << "A=M-1\n";

        if(command == "neg") ofs << "M=-M\n";
        if(command == "not") ofs << "M=!M\n";
    }
    // 二項演算子
    else{
        ofs << "@SP\n";
        ofs << "AM=M-1\n";
        ofs << "D=M\n";
        ofs << "A=A-1\n";
        // x: M, y: D

        // 二項演算子
        if(command == "add" || command == "sub" || command == "and" || command == "or"){
            if(command == "add") ofs << "M=D+M\n";
            if(command == "sub") ofs << "M=M-D\n";
            if(command == "and") ofs << "M=D&M\n";
            if(command == "or") ofs << "M=D|M\n";
        }
        // 比較演算子
        else{
            string trueLabel = newLabel();
            string endLabel = newLabel();

            ofs << "D=M-D\n";
            // Jump if True
            ofs << "@" << trueLabel << "\n";
            if(command == "eq") ofs << "D;JEQ\n";
            if(command == "gt") ofs << "D;JGT\n";
            if(command == "lt") ofs << "D;JLT\n";
            // if False
            ofs << "D=0\n";
            ofs << "@" << endLabel << "\n";
            ofs << "0;JMP\n";
            // if True
            ofs << "(" << trueLabel << ")\n";
            ofs << "D=-1\n";
            ofs << "(" << endLabel << ")\n";

            ofs << "@SP\n";
            ofs << "A=M-1\n";
            ofs << "M=D\n";
        }
    }
}

void CodeWriter::writePushPop(string command, string segment, int index){
    // Pushコマンド
    if(command == "C_PUSH"){
        if(segment == "constant"){
            ofs << "@" << index << "\n";
            ofs << "D=A\n";
        }
        if(segment == "local"){
            ofs << "@" << index << "\n";
            ofs << "D=A\n";
            ofs << "@LCL\n";
            ofs << "A=D+M\n";
            ofs << "D=M\n";
        }
        if(segment == "argument"){
            ofs << "@" << index << "\n";
            ofs << "D=A\n";
            ofs << "@ARG\n";
            ofs << "A=D+M\n";
            ofs << "D=M\n";
        }
        if(segment == "this"){
            ofs << "@" << index << "\n";
            ofs << "D=A\n";
            ofs << "@THIS\n";
            ofs << "A=D+M\n";
            ofs << "D=M\n";
        }
        if(segment == "that"){
            ofs << "@" << index << "\n";
            ofs << "D=A\n";
            ofs << "@THAT\n";
            ofs << "A=D+M\n";
            ofs << "D=M\n";
        }
        if(segment == "pointer"){
            ofs << "@R" << 3 + index << "\n";
            ofs << "D=M\n";
        }
        if(segment == "temp"){
            ofs << "@R" << 5 + index << "\n";
            ofs << "D=M\n";
        }
        if(segment == "static"){
            ofs << "@"<< fileName << "." << index << "\n";
            ofs << "D=M\n";
        }

        // スタックにPush
        ofs << "@SP\n";
        ofs << "M=M+1\n";
        ofs << "A=M-1\n";
        ofs << "M=D\n";
    }
    // Popコマンド
    else{
        // スタックからPop
        ofs << "@SP\n";
        ofs << "AM=M-1\n";
        ofs << "D=M\n";

        if(segment == "local"){
            ofs << "@R13\n";
            ofs << "M=D\n";
            ofs << "@LCL\n";
            ofs << "D=M\n";
            ofs << "@" << index << "\n";
            ofs << "D=D+A\n";
            ofs << "@R14\n";
            ofs << "M=D\n";
            ofs << "@R13\n";
            ofs << "D=M\n";
            ofs << "@R14\n";
            ofs << "A=M\n";
            ofs << "M=D\n";
        }
        if(segment == "argument"){
            ofs << "@R13\n";
            ofs << "M=D\n";
            ofs << "@ARG\n";
            ofs << "D=M\n";
            ofs << "@" << index << "\n";
            ofs << "D=D+A\n";
            ofs << "@R14\n";
            ofs << "M=D\n";
            ofs << "@R13\n";
            ofs << "D=M\n";
            ofs << "@R14\n";
            ofs << "A=M\n";
            ofs << "M=D\n";
        }
        if(segment == "this"){
            ofs << "@R13\n";
            ofs << "M=D\n";
            ofs << "@THIS\n";
            ofs << "D=M\n";
            ofs << "@" << index << "\n";
            ofs << "D=D+A\n";
            ofs << "@R14\n";
            ofs << "M=D\n";
            ofs << "@R13\n";
            ofs << "D=M\n";
            ofs << "@R14\n";
            ofs << "A=M\n";
            ofs << "M=D\n";
        }
        if(segment == "that"){
            ofs << "@R13\n";
            ofs << "M=D\n";
            ofs << "@THAT\n";
            ofs << "D=M\n";
            ofs << "@" << index << "\n";
            ofs << "D=D+A\n";
            ofs << "@R14\n";
            ofs << "M=D\n";
            ofs << "@R13\n";
            ofs << "D=M\n";
            ofs << "@R14\n";
            ofs << "A=M\n";
            ofs << "M=D\n";
        }
        if(segment == "pointer"){
            ofs << "@R" << 3 + index << "\n";
            ofs << "M=D\n";
        }
        if(segment == "temp"){
            ofs << "@R" << 5 + index << "\n";
            ofs << "M=D\n";
        }
        if(segment == "static"){
            ofs << "@" << fileName << "." << index << "\n";
            ofs << "M=D\n";
        }
    }
}

void CodeWriter::close(){
    ofs.close();
}

void CodeWriter::writeLabel(string label){
    ofs << "(" + functionName + "$" + label + ")\n";
}

void CodeWriter::writeGoto(string label){
    ofs << "@" + functionName + "$" + label + "\n";
    ofs << "0;JMP\n";
}

void CodeWriter::writeIf(string label){
    ofs << "@SP\n";
    ofs << "AM=M-1\n";
    ofs << "D=M\n";
    ofs << "@" + functionName + "$" + label + "\n";
    ofs << "D;JNE\n";
}

void CodeWriter::writeReturn(){
    // FRAME = LCL
    ofs << "@LCL\n";
    ofs << "D=M\n";
    ofs << "@R13\n";
    ofs << "M=D\n";

    // RET = *(FRAME-5)
    ofs << "@5\n";
    ofs << "D=A\n";
    ofs << "@LCL\n";
    ofs << "A=M-D\n";
    ofs << "D=M\n";
    ofs << "@R14\n";
    ofs << "M=D\n";

    // *ARG = pop()
    ofs << "@SP\n";
    ofs << "A=M-1\n";
    ofs << "D=M\n";
    ofs << "@ARG\n";
    ofs << "A=M\n";
    ofs << "M=D\n";

    // SP = ARG+1
    ofs << "@ARG\n";
    ofs << "D=M\n";
    ofs << "@SP\n";
    ofs << "M=D+1\n";

    // THAT = *(FRAME-1)
    ofs << "@R13\n";
    ofs << "MD=M-1\n";
    ofs << "A=D\n";
    ofs << "D=M\n";
    ofs << "@THAT\n";
    ofs << "M=D\n";

    // THIS = *(FRAME-2)
    ofs << "@R13\n";
    ofs << "MD=M-1\n";
    ofs << "A=D\n";
    ofs << "D=M\n";
    ofs << "@THIS\n";
    ofs << "M=D\n";

    // ARG = *(FRAME-3)
    ofs << "@R13\n";
    ofs << "MD=M-1\n";
    ofs << "A=D\n";
    ofs << "D=M\n";
    ofs << "@ARG\n";
    ofs << "M=D\n";

    // LCL = *(FRAME-4)
    ofs << "@R13\n";
    ofs << "MD=M-1\n";
    ofs << "A=D\n";
    ofs << "D=M\n";
    ofs << "@LCL\n";
    ofs << "M=D\n";

    // goto RET
    ofs << "@R14\n";
    ofs << "A=M\n";
}

void CodeWriter::writeFunction(string functionName, int numLocals){
    this->functionName = functionName;

    // (f)
    ofs << "(" + functionName + ")\n";

    // repeat k times:
    // push 0
    ofs << "@0\n";
    ofs << "D=A\n";
    ofs << "@SP\n";
    ofs << "A=M\n";
    for(int i = 0; i < numLocals; ++i){
        ofs << "M=D\n";
        ofs << "A=A+1\n";
    }
    ofs << "@" << numLocals << "\n";
    ofs << "D=A\n";
    ofs << "@SP\n";
    ofs << "M=D+M\n";
}

void CodeWriter::writeCode(ParseElement e){
    if(e.commandType == "C_ARITHMETIC") writeArithmetic(e.arg1);
    if(e.commandType == "C_PUSH") writePushPop(e.commandType, e.arg1, e.arg2);
    if(e.commandType == "C_POP") writePushPop(e.commandType, e.arg1, e.arg2);
    if(e.commandType == "C_LABEL") writeLabel(e.arg1);
    if(e.commandType == "C_GOTO") writeGoto(e.arg1);
    if(e.commandType == "C_IF") writeIf(e.arg1);
    if(e.commandType == "C_RETURN") writeReturn();
    if(e.commandType == "C_FUNCTION") writeFunction(e.arg1, e.arg2);
}

string CodeWriter::newLabel(){
    return "LABEL" + to_string(++label);
}