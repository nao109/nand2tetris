#include "VMtranslator.hpp"

int CodeWriter::label = 0;

void CodeWriter::setInputFileName(fs::path fileName){
    this->inputFileName = fileName;
}

ofstream CodeWriter::setFileName(fs::path fileName){
    string outputFileName = fileName.string();
    if(!fs::is_directory(fileName)){
        outputFileName.replace(outputFileName.rfind(".vm"), 3, ".asm");
    }
    else{
        outputFileName.append(".asm");
    }
    ofstream file(outputFileName);

    // SP = 256
    file << "@256\n";
    file << "D=A\n";
    file << "@SP\n";
    file << "M=D\n";

    return file;
}

void CodeWriter::writeArithmetic(ofstream &file, ParseElement e){
    // 算術コマンドでなければパス
    if(e.commandType != "C_ARITHMETIC") return;

    string command = e.arg1;

    // 単項演算子
    if(command == "neg" || command == "not"){
        file << "@SP\n";
        file << "A=M-1\n";

        if(command == "neg") file << "M=-M\n";
        if(command == "not") file << "M=!M\n";
    }
    // 二項演算子
    else{
        file << "@SP\n";
        file << "AM=M-1\n";
        file << "D=M\n";
        file << "A=A-1\n";
        // x: M, y: D

        // 二項演算子
        if(command == "add" || command == "sub" || command == "and" || command == "or"){
            if(command == "add") file << "M=D+M\n";
            if(command == "sub") file << "M=M-D\n";
            if(command == "and") file << "M=D&M\n";
            if(command == "or") file << "M=D|M\n";
        }
        // 比較演算子
        else{
            string trueLabel = newLabel();
            string endLabel = newLabel();

            file << "D=M-D\n";
            // Jump if True
            file << "@" << trueLabel << "\n";
            if(command == "eq") file << "D;JEQ\n";
            if(command == "gt") file << "D;JGT\n";
            if(command == "lt") file << "D;JLT\n";
            // if False
            file << "D=0\n";
            file << "@" << endLabel << "\n";
            file << "0;JMP\n";
            // if True
            file << "(" << trueLabel << ")\n";
            file << "D=-1\n";
            file << "(" << endLabel << ")\n";

            file << "@SP\n";
            file << "A=M-1\n";
            file << "M=D\n";
        }
    }
}

void CodeWriter::writePushPop(ofstream &file, ParseElement e){
    // Pushコマンド、Popコマンドでなければパス
    if(e.commandType != "C_PUSH" && e.commandType != "C_POP") return;

    string command = e.commandType;
    string arg1 = e.arg1;
    int arg2 = e.arg2;

    // Pushコマンド
    if(command == "C_PUSH"){
        if(arg1 == "constant"){
            file << "@" << arg2 << "\n";
            file << "D=A\n";
        }
        if(arg1 == "local"){
            file << "@" << arg2 << "\n";
            file << "D=A\n";
            file << "@LCL\n";
            file << "A=D+M\n";
            file << "D=M\n";
        }
        if(arg1 == "argument"){
            file << "@" << arg2 << "\n";
            file << "D=A\n";
            file << "@ARG\n";
            file << "A=D+M\n";
            file << "D=M\n";
        }
        if(arg1 == "this"){
            file << "@" << arg2 << "\n";
            file << "D=A\n";
            file << "@THIS\n";
            file << "A=D+M\n";
            file << "D=M\n";
        }
        if(arg1 == "that"){
            file << "@" << arg2 << "\n";
            file << "D=A\n";
            file << "@THAT\n";
            file << "A=D+M\n";
            file << "D=M\n";
        }
        if(arg1 == "pointer"){
            file << "@R" << 3 + arg2 << "\n";
            file << "D=M\n";
        }
        if(arg1 == "temp"){
            file << "@R" << 5 + arg2 << "\n";
            file << "D=M\n";
        }
        if(arg1 == "static"){
            file << "@"<< inputFileName << "." << arg2 << "\n";
            file << "D=M\n";
        }

        // スタックにPush
        file << "@SP\n";
        file << "M=M+1\n";
        file << "A=M-1\n";
        file << "M=D\n";
    }
    // Popコマンド
    else{
        // スタックからPop
        file << "@SP\n";
        file << "AM=M-1\n";
        file << "D=M\n";

        if(arg1 == "local"){
            file << "@R13\n";
            file << "M=D\n";
            file << "@LCL\n";
            file << "D=M\n";
            file << "@" << arg2 << "\n";
            file << "D=D+A\n";
            file << "@R14\n";
            file << "M=D\n";
            file << "@R13\n";
            file << "D=M\n";
            file << "@R14\n";
            file << "A=M\n";
            file << "M=D\n";
        }
        if(arg1 == "argument"){
            file << "@R13\n";
            file << "M=D\n";
            file << "@ARG\n";
            file << "D=M\n";
            file << "@" << arg2 << "\n";
            file << "D=D+A\n";
            file << "@R14\n";
            file << "M=D\n";
            file << "@R13\n";
            file << "D=M\n";
            file << "@R14\n";
            file << "A=M\n";
            file << "M=D\n";
        }
        if(arg1 == "this"){
            file << "@R13\n";
            file << "M=D\n";
            file << "@THIS\n";
            file << "D=M\n";
            file << "@" << arg2 << "\n";
            file << "D=D+A\n";
            file << "@R14\n";
            file << "M=D\n";
            file << "@R13\n";
            file << "D=M\n";
            file << "@R14\n";
            file << "A=M\n";
            file << "M=D\n";
        }
        if(arg1 == "that"){
            file << "@R13\n";
            file << "M=D\n";
            file << "@THAT\n";
            file << "D=M\n";
            file << "@" << arg2 << "\n";
            file << "D=D+A\n";
            file << "@R14\n";
            file << "M=D\n";
            file << "@R13\n";
            file << "D=M\n";
            file << "@R14\n";
            file << "A=M\n";
            file << "M=D\n";
        }
        if(arg1 == "pointer"){
            file << "@R" << 3 + arg2 << "\n";
            file << "M=D\n";
        }
        if(arg1 == "temp"){
            file << "@R" << 5 + arg2 << "\n";
            file << "M=D\n";
        }
        if(arg1 == "static"){
            file << "@" << inputFileName << "." << arg2 << "\n";
            file << "M=D\n";
        }
    }
}

void CodeWriter::close(ofstream &file){
    file.close();
}

string CodeWriter::newLabel(){
    return "LABEL" + to_string(++label);
}
