#include "VMtranslator.hpp"

VMtranslator::VMtranslator(fs::path inputFile) : inputFile(inputFile) {}

void VMtranslator::translateFile(CodeWriter &codewriter, fs::path inputFile){
    Parser parser(inputFile);

    codewriter.setFileName(inputFile.string());

    vector<ParseElement> vpe = parser.parse();

    for(auto e : vpe) codewriter.writeCode(e);
}

void VMtranslator::translate(){
    CodeWriter codewriter(inputFile);

    // vmファイルが入力された場合
    if(!fs::is_directory(inputFile)){
        translateFile(codewriter, inputFile);
    }
    // ディレクトリが入力された場合
    else{
        // 入力ファイル
        for(auto x : fs::directory_iterator(inputFile)){
            string fileName = x.path().string();

            // vmファイル以外は無視
            if(fileName.rfind(".vm") == string::npos) continue;

            translateFile(codewriter, fileName);
        }
    }
    codewriter.close();
}
