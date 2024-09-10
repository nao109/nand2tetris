#include "JackCompiler.hpp"

JackCompiler::JackCompiler(std::filesystem::path inputFile) : inputFile(inputFile) {}

void JackCompiler::compileFile(std::filesystem::path inputFile){
    CompilationEngine compilationEngine(inputFile);
    compilationEngine.compile();
}

void JackCompiler::compile(){
    // ディレクトリが入力された場合
    if(std::filesystem::is_directory(inputFile)){
        // 入力ファイル
        for(auto file : std::filesystem::directory_iterator(inputFile)){
            std::string fileName = file.path().string();

            // jackファイル以外は無視
            if(fileName.rfind(".jack") != std::string::npos) compileFile(fileName);
        }
    }
    // jackファイルが入力された場合
    else{
        std::string fileName = inputFile.string();

        // jackファイル以外は無視
        if(fileName.rfind(".jack") != std::string::npos) compileFile(fileName);
    }
}
