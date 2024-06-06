#include "VMtranslator.hpp"

int VMtranslator::CodeWriter::label = 0;

ofstream VMtranslator::CodeWriter::setFileName(fs::path fileName){
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

void VMtranslator::CodeWriter::writeArithmetic(ofstream &file, ParseElement e){
    // 算術コマンドでなければパス
    if(e.commandType != "C_ARITHMETIC") return;

    string command = e.arg1;

    // 単項演算子
    if(command == "neg" || command == "not"){
        file << "@SP\n";
        file << "M=M-1\n";
        file << "A=M\n";

        if(command == "neg") file << "M=-M\n";
        if(command == "not") file << "M=!M\n";

        file << "@SP\n";
        file << "M=M+1\n";
    }
    // 二項演算子
    else{
        file << "@SP\n";
        file << "M=M-1\n";
        file << "A=M\n";
        file << "D=M\n";
        file << "@SP\n";
        file << "M=M-1\n";
        file << "A=M\n";
        // x: M, y: D

        // 二項演算子
        if(command == "add" || command == "sub" || command == "and" || command == "or"){
            if(command == "add") file << "D=D+M\n";
            if(command == "sub") file << "D=M-D\n";
            if(command == "and") file << "D=D&M\n";
            if(command == "or") file << "D=D|M\n";
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
        }

        file << "@SP\n";
        file << "A=M\n";
        file << "M=D\n";
        file << "@SP\n";
        file << "M=M+1\n";
    }
}

void VMtranslator::CodeWriter::writePushPop(ofstream &file, ParseElement e){
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

        // スタックにPush
        file << "@SP\n";
        file << "A=M\n";
        file << "M=D\n";
        file << "@SP\n";
        file << "M=M+1\n";
    }
    // Popコマンド
    else{
    }
}

void VMtranslator::CodeWriter::close(ofstream &file){
    file.close();
}

string VMtranslator::CodeWriter::newLabel(){
    return "LABEL" + to_string(++label);
}
