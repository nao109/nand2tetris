#ifndef VMTRANSLATOR_H_
#define VMTRANSLATOR_H_

#include <bits/stdc++.h>
using namespace std;

namespace fs = filesystem;

class VMtranslator {
    struct ParseElement {
        string commandType;
        string arg1;
        int arg2;
    };

    class Parser {
    public:
        string commandType(const vector<string> &v);

        string arg1(const vector<string> &v);

        int arg2(const vector<string> &v);

        vector<ParseElement> parse(ifstream &ifs);   
    };

    class CodeWriter {
        static int label;

    public:
        ofstream setFileName(fs::path fileName);

        void writeArithmetic(ofstream &file, ParseElement e);

        void writePushPop(ofstream &file, ParseElement e);

        void close(ofstream &file);

        string newLabel();
    };

    Parser parser;
    CodeWriter codewriter;

    void translateFile(fs::path inputFile, ofstream &outputFile){
        // 入力vmファイル
        ifstream ifs(inputFile);
        if(ifs.fail()){
            cerr << "Cannot open file: " << inputFile << "\n";
            return;
        }

        vector<ParseElement> vpe = parser.parse(ifs);

        for(auto e : vpe){
            codewriter.writeArithmetic(outputFile, e);
            codewriter.writePushPop(outputFile, e);
        }
    }

public:
    void translate(fs::path inputFile){
        // 出力asmファイル
        ofstream outputFile = codewriter.setFileName(inputFile);

        // vmファイルが入力された場合
        if(!fs::is_directory(inputFile)){
            translateFile(inputFile, outputFile);
        }
        // ディレクトリが入力された場合
        else{
            // 入力ファイル
            for(auto x : fs::directory_iterator(inputFile)){
                string fileName = x.path().string();

                // vmファイル以外は無視
                if(fileName.rfind(".vm") == string::npos) continue;

                translateFile(fileName, outputFile);
            }
        }
        codewriter.close(outputFile);
    }
};

#endif  // VMTRANSLATOR_H_
