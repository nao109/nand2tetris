#include "Parser.hpp"

void Parser::advance(){
    address++;
}

int Parser::getAddress(){
    return address;
}

string Parser::commandType(string str){
    string res;
    if(str[0] == '@') res = "A_COMMAND";
    else if(str[0] == '(') res = "L_COMMAND";
    else res = "C_COMMAND";
    return res;
}

string Parser::symbol(string str){
    string res;
    if(commandType(str) == "A_COMMAND"){
        res = str.substr(1, str.size() - 2);
    }
    else if(commandType(str) == "L_COMMAND"){
        res = str.substr(1, str.size() - 3);
    }
    else res = "";
    return res;
}

string Parser::dest(string str){
    int pos = str.find("=");
    string res;
    if(pos == -1) res = "";
    else res = str.substr(0, pos);
    return res;
}

string Parser::comp(string str){
    int pos1 = str.find("=");
    int pos2 = str.find(";");
    string res;
    if(pos1 == -1 && pos2 == -1) res = "";
    else if(pos1 != -1 && pos2 != -1) res = str.substr(pos1 + 1, pos2 - pos1);
    else if(pos1 != -1 && pos2 == -1) res = str.substr(pos1 + 1);
    else res = str.substr(0, pos2 + 1);
    res.pop_back();
    return res;
}

string Parser::jump(string str){
    int pos = str.find(";");
    string res;
    if(pos == -1) return "";
    else res = str.substr(pos + 1);
    res.pop_back();
    return res;
}
