#include "JackAnalyzer.hpp"

JackAnalyzer::JackAnalyzer(fs::path inputFile) : inputFile(inputFile) {}

void JackAnalyzer::analyzeFile(fs::path inputFile){
    CompilationEngine compilationEngine(inputFile);
    compilationEngine.compile();
}

void JackAnalyzer::analyze(){
    // ディレクトリが入力された場合
    if(fs::is_directory(inputFile)){
        // 入力ファイル
        for(auto file : fs::directory_iterator(inputFile)){
            string fileName = file.path().string();

            // jackファイル以外は無視
            if(fileName.rfind(".jack") != string::npos) analyzeFile(fileName);
        }
    }
    // jackファイルが入力された場合
    else{
        string fileName = inputFile.string();

        // jackファイル以外は無視
        if(fileName.rfind(".jack") != string::npos) analyzeFile(fileName);
    }
}
