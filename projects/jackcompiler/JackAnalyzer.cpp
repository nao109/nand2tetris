#include "JackAnalyzer.hpp"

JackAnalyzer::JackAnalyzer(std::filesystem::path inputFile) : inputFile(inputFile) {}

void JackAnalyzer::analyzeFile(std::filesystem::path inputFile){
    CompilationEngine compilationEngine(inputFile);
    compilationEngine.compile();
}

void JackAnalyzer::analyze(){
    // ディレクトリが入力された場合
    if(std::filesystem::is_directory(inputFile)){
        // 入力ファイル
        for(auto file : std::filesystem::directory_iterator(inputFile)){
            std::string fileName = file.path().string();

            // jackファイル以外は無視
            if(fileName.rfind(".jack") != std::string::npos) analyzeFile(fileName);
        }
    }
    // jackファイルが入力された場合
    else{
        std::string fileName = inputFile.string();

        // jackファイル以外は無視
        if(fileName.rfind(".jack") != std::string::npos) analyzeFile(fileName);
    }
}
