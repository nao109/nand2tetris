#include <bits/stdc++.h>
using namespace std;

#include "Assembler.hpp"
using namespace Assembler;

int main(int argc, char *argv[]) {
    if(argc != 2){
        cerr << "Cannot start program\n";
        return 1;
    }

    string input, output;
    input = output = argv[1];
    output.replace(output.rfind("asm"), 3, "hack");

    ifstream ifs(input);
    if(ifs.fail()){
       cerr << "Cannot open file\n";
       return 1;
    }

    //
    string str;
    vector<string> s;
    while(getline(ifs, str)){
        if(str[1] == '\0') continue;

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

    SymbolTable symboltable;
    symboltable.next_address = 16;
    for(string sstr : s){
        string commandType = Parser::commandType(sstr);
        string symbol = Parser::symbol(sstr);


        if(commandType == "L_COMMAND"){
            if(!symboltable.contains(symbol)){
                symboltable.addEntry(symbol, Parser::address);
            }
        }
        else Parser::address++;
    }

    vector<string> decode;
    Parser::address = 0;
    for(string str : s){
        ParseElement e;
        e.commandType = Parser::commandType(str);
        if(e.commandType == "A_COMMAND"){
            e.symbol = Parser::symbol(str);
        }
        else if(e.commandType == "L_COMMAND"){
            e.symbol = Parser::symbol(str);
        }
        else if(e.commandType == "C_COMMAND"){
            e.dest = Parser::dest(str);
            e.comp = Parser::comp(str);
            e.jump = Parser::jump(str);
        }

        if(e.commandType == "C_COMMAND"){
            decode.push_back("111" + Code::comp(e.comp) + Code::dest(e.dest) + Code::jump(e.jump));
        }
        else if(e.commandType == "A_COMMAND"){
            if(isalpha(e.symbol[0])){
                if(symboltable.contains(e.symbol)) e.address = symboltable.getAddress(e.symbol);
                else{
                    symboltable.addEntry(e.symbol, symboltable.next_address);
                    e.address = symboltable.getAddress(e.symbol);
                    symboltable.next_address++;
                }
            }
            else e.address = stoi(e.symbol);
            bitset<15> bs(e.address);
            decode.push_back("0" + bs.to_string());
        }
    }

    ofstream file(output);
    for(string s : decode) file << s << endl;
    return 0;
}
