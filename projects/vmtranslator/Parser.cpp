#include "VMtranslator.hpp"

map<const string, string> commandTypeTable = {
    {     "add", "C_ARITHMETIC"},
    {     "sub", "C_ARITHMETIC"},
    {     "neg", "C_ARITHMETIC"},
    {      "eq", "C_ARITHMETIC"},
    {      "gt", "C_ARITHMETIC"},
    {      "lt", "C_ARITHMETIC"},
    {     "and", "C_ARITHMETIC"},
    {      "or", "C_ARITHMETIC"},
    {     "not", "C_ARITHMETIC"},
    {    "push",       "C_PUSH"},
    {     "pop",        "C_POP"},
    {   "label",      "C_LABEL"},
    {    "goto",       "C_GOTO"},
    { "if-goto",         "C_IF"},
    {"function",   "C_FUNCTION"},
    {  "return",     "C_RETURN"},
    {    "call",       "C_CALL"}
};

vector<string> split(const string& s){
    vector<string> v;
    auto p = string::size_type(0);
    while(1){
        /// split with separator ///
        auto pos = s.find(' ', p);
        if(pos == string::npos){
            if(p == s.size()) break;
            v.emplace_back(s.begin() + p, s.end());
            break;
        }
        if(p != pos) v.emplace_back(s.begin() + p, s.begin() + pos);
        p = pos + 1;
    }
    return v;
}

string Parser::commandType(const vector<string> &v){
    if(commandTypeTable.count(v[0])) return commandTypeTable[v[0]];
    else return "";
}

string Parser::arg1(const vector<string> &v){
    if(commandType(v) == "C_RETURN") return "";
    else if(commandType(v) == "C_ARITHMETIC") return v[0];
    else return v[1];
}

int Parser::arg2(const vector<string> &v){
    int res;
    if(commandType(v) == "C_PUSH" || commandType(v) == "C_POP" || commandType(v) == "C_FUNCTION" || commandType(v) == "C_CALL"){
        res = stoi(v[2]);
    }
    else res = 0;
    return res;
}

vector<ParseElement> Parser::parse(ifstream &ifs){
    vector<ParseElement> vpe;
    // VMの入力
    string str;
    vector<string> vm_code; // VMのコード

    while(getline(ifs, str)){
        string buf;
        // CRは無視
        for(char i : str){
            if(i != '\r') buf.push_back(i);
        }
        // スラッシュは無視
        int slash = 0;
        string sstr;
        for(char i : buf){
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
        // 空文字列でなければpush back
        if(!sstr.empty()) vm_code.push_back(sstr);
    }

    for(string s : vm_code){
        vector<string> code = split(s);
        ParseElement e;
        e.commandType = commandType(code);
        e.arg1 = arg1(code);
        e.arg2 = arg2(code);
        vpe.push_back(e);
    }
    return vpe;
}
