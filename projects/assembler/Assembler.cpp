#include "Assembler.hpp"

vector<string> Assembler::inputFile(string input_file){
    // アセンブリの入力

    // ifstreamで読み込む
    ifstream ifs(input_file);

    string str;
    vector<string> s; // アセンブリのコード
    while(getline(ifs, str)){
        // 空行は無視
        if(str[1] == '\0') continue;

        // 空白は無視
        int slash = 0;
        string sstr;
        for(char i : str){
            if(i == ' ') continue;

            if(i == '/'){
                if(slash == 0) slash++;
                else break;
            }
            else if(slash == 1){
                sstr.push_back('/');
                slash = 0;
            }
            if(slash == 0) sstr.push_back(i);
        }
        if(sstr.empty()) continue;
        s.push_back(sstr);
    }
    return s;
}

void Assembler::outputFile(string output_file, vector<string> decode){
    // hackコードの出力
    ofstream file(output_file);
    for(string str : decode) file << str << "\n";
}

void Assembler::assemble(string input_file, string output_file){
    vector<string> asm_code = inputFile(input_file); // アセンブリのコード

    // Parser
    Parser parser;
    // Coder
    Code code;
    // シンボルとアドレスの対応表
    SymbolTable symboltable;
    for(string sstr : asm_code){
        string commandType = parser.commandType(sstr);
        string symbol = parser.symbol(sstr);

        // L_COMMANDなら対応表に追加
        if(commandType == "L_COMMAND"){
            if(!symboltable.contains(symbol)){
                symboltable.addEntry(symbol, parser.getAddress());
            }
        }
        // それ以外はアドレスをインクリメント
        else parser.advance();
    }

    // hackコード
    vector<string> decode;
    for(string str : asm_code){
        ParseElement e;
        e.commandType = parser.commandType(str);
        if(e.commandType == "A_COMMAND"){
            e.symbol = parser.symbol(str);
        }
        else if(e.commandType == "L_COMMAND"){
            e.symbol = parser.symbol(str);
        }
        else if(e.commandType == "C_COMMAND"){
            e.dest = parser.dest(str);
            e.comp = parser.comp(str);
            e.jump = parser.jump(str);
        }

        if(e.commandType == "C_COMMAND"){
            decode.push_back("111" + code.comp(e.comp) + code.dest(e.dest) + code.jump(e.jump));
        }
        else if(e.commandType == "A_COMMAND"){
            // シンボルがアドレスでない場合
            if(isalpha(e.symbol[0])){
                if(symboltable.contains(e.symbol)) e.address = symboltable.getAddress(e.symbol);
                else{
                    // シンボルとアドレスを対応付け
                    symboltable.addEntry(e.symbol, symboltable.getNextAddress());
                    e.address = symboltable.getAddress(e.symbol);
                    symboltable.incNextAddress();
                }
            }
            // シンボルがアドレスである場合
            else e.address = stoi(e.symbol);
            bitset<15> bs(e.address);
            decode.push_back("0" + bs.to_string());
        }
    }

    outputFile(output_file, decode);
}
