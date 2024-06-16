#ifndef VMTRANSLATOR_H_
#define VMTRANSLATOR_H_

#include <bits/stdc++.h>
using namespace std;

namespace fs = filesystem;

struct ParseElement {
    string commandType;
    string arg1;
    int arg2;
};

class Parser {
    ifstream ifs;

public:
    Parser(fs::path file);
    string commandType(const vector<string> &v);
    string arg1(const vector<string> &v);
    int arg2(const vector<string> &v);
    vector<ParseElement> parse();
};

class CodeWriter {
    ofstream ofs;
    string fileName;
    static int label;

public:
    CodeWriter(fs::path file);
    void setFileName(string fileName);
    void writeArithmetic(ParseElement e);
    void writePushPop(ParseElement e);
    void close();
    string newLabel();
};

class VMtranslator {
    fs::path inputFile;

    void translateFile(CodeWriter &codewriter, fs::path inputFile){
        Parser parser(inputFile);

        codewriter.setFileName(inputFile.string());

        vector<ParseElement> vpe = parser.parse();

        for(auto e : vpe){
            codewriter.writeArithmetic(e);
            codewriter.writePushPop(e);
        }
    }

public:
    VMtranslator(fs::path inputFile) : inputFile(inputFile) {}

    void translate(){
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
};

#endif  // VMTRANSLATOR_H_
