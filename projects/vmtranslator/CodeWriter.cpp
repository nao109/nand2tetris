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

    // SP = 256
    ofs << "@256\n";
    ofs << "D=A\n";
    ofs << "@SP\n";
    ofs << "M=D\n";
}

void CodeWriter::setFileName(string fileName){
    this->fileName = fileName.substr(0, fileName.rfind(".vm"));
}

void CodeWriter::writeArithmetic(ParseElement e){
    // 算術コマンドでなければパス
    if(e.commandType != "C_ARITHMETIC") return;

    string command = e.arg1;

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

void CodeWriter::writePushPop(ParseElement e){
    // Pushコマンド、Popコマンドでなければパス
    if(e.commandType != "C_PUSH" && e.commandType != "C_POP") return;

    string command = e.commandType;
    string arg1 = e.arg1;
    int arg2 = e.arg2;

    // Pushコマンド
    if(command == "C_PUSH"){
        if(arg1 == "constant"){
            ofs << "@" << arg2 << "\n";
            ofs << "D=A\n";
        }
        if(arg1 == "local"){
            ofs << "@" << arg2 << "\n";
            ofs << "D=A\n";
            ofs << "@LCL\n";
            ofs << "A=D+M\n";
            ofs << "D=M\n";
        }
        if(arg1 == "argument"){
            ofs << "@" << arg2 << "\n";
            ofs << "D=A\n";
            ofs << "@ARG\n";
            ofs << "A=D+M\n";
            ofs << "D=M\n";
        }
        if(arg1 == "this"){
            ofs << "@" << arg2 << "\n";
            ofs << "D=A\n";
            ofs << "@THIS\n";
            ofs << "A=D+M\n";
            ofs << "D=M\n";
        }
        if(arg1 == "that"){
            ofs << "@" << arg2 << "\n";
            ofs << "D=A\n";
            ofs << "@THAT\n";
            ofs << "A=D+M\n";
            ofs << "D=M\n";
        }
        if(arg1 == "pointer"){
            ofs << "@R" << 3 + arg2 << "\n";
            ofs << "D=M\n";
        }
        if(arg1 == "temp"){
            ofs << "@R" << 5 + arg2 << "\n";
            ofs << "D=M\n";
        }
        if(arg1 == "static"){
            ofs << "@"<< fileName << "." << arg2 << "\n";
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

        if(arg1 == "local"){
            ofs << "@R13\n";
            ofs << "M=D\n";
            ofs << "@LCL\n";
            ofs << "D=M\n";
            ofs << "@" << arg2 << "\n";
            ofs << "D=D+A\n";
            ofs << "@R14\n";
            ofs << "M=D\n";
            ofs << "@R13\n";
            ofs << "D=M\n";
            ofs << "@R14\n";
            ofs << "A=M\n";
            ofs << "M=D\n";
        }
        if(arg1 == "argument"){
            ofs << "@R13\n";
            ofs << "M=D\n";
            ofs << "@ARG\n";
            ofs << "D=M\n";
            ofs << "@" << arg2 << "\n";
            ofs << "D=D+A\n";
            ofs << "@R14\n";
            ofs << "M=D\n";
            ofs << "@R13\n";
            ofs << "D=M\n";
            ofs << "@R14\n";
            ofs << "A=M\n";
            ofs << "M=D\n";
        }
        if(arg1 == "this"){
            ofs << "@R13\n";
            ofs << "M=D\n";
            ofs << "@THIS\n";
            ofs << "D=M\n";
            ofs << "@" << arg2 << "\n";
            ofs << "D=D+A\n";
            ofs << "@R14\n";
            ofs << "M=D\n";
            ofs << "@R13\n";
            ofs << "D=M\n";
            ofs << "@R14\n";
            ofs << "A=M\n";
            ofs << "M=D\n";
        }
        if(arg1 == "that"){
            ofs << "@R13\n";
            ofs << "M=D\n";
            ofs << "@THAT\n";
            ofs << "D=M\n";
            ofs << "@" << arg2 << "\n";
            ofs << "D=D+A\n";
            ofs << "@R14\n";
            ofs << "M=D\n";
            ofs << "@R13\n";
            ofs << "D=M\n";
            ofs << "@R14\n";
            ofs << "A=M\n";
            ofs << "M=D\n";
        }
        if(arg1 == "pointer"){
            ofs << "@R" << 3 + arg2 << "\n";
            ofs << "M=D\n";
        }
        if(arg1 == "temp"){
            ofs << "@R" << 5 + arg2 << "\n";
            ofs << "M=D\n";
        }
        if(arg1 == "static"){
            ofs << "@" << fileName << "." << arg2 << "\n";
            ofs << "M=D\n";
        }
    }
}

void CodeWriter::close(){
    ofs.close();
}

string CodeWriter::newLabel(){
    return "LABEL" + to_string(++label);
}