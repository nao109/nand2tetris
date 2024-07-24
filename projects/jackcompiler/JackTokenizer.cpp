#include "JackTokenizer.hpp"

set<string> keywords = {
    "class", "method", "function", "constructor", "int", "boolean", "char", "void", "var","static",
    "field", "let", "do", "if", "else", "while", "return", "true", "false", "null", "this"
};
set<char> symbols = {'{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~'};

JackTokenizer::JackTokenizer(fs::path inputFile) {
    // 入力jackファイル
    ifs.open(inputFile);
    if(ifs.fail()){
        cerr << "Cannot open file: " << inputFile.string() << "\n";
    }

    // 初期化
    tokenVal = "";
    tokens.clear();
    id = 0;

    tokenize();
    printTokens(inputFile);
}

bool JackTokenizer::hasMoreTokens(){
    return id < tokens.size();
}

void JackTokenizer::advance(){
    ++id;
}

TokenType JackTokenizer::tokenType(){
    return tokens[id].tokenType;
}

string JackTokenizer::keyword(){
    return tokens[id].keyword.value();
}

char JackTokenizer::symbol(){
    return tokens[id].symbol.value();
}

string JackTokenizer::identifier(){
    return tokens[id].identifier.value();
}

int JackTokenizer::intVal(){
    return tokens[id].intVal.value();
}

string JackTokenizer::stringVal(){
    return tokens[id].stringVal.value();
}

TokenType JackTokenizer::peekTokenType(){
    return tokens[id + 1].tokenType;
}

char JackTokenizer::peekSymbol(){
    return tokens[id + 1].symbol.value();
}

void JackTokenizer::printTokens(fs::path inputFile){
    string TokensFile = inputFile.string();
    TokensFile.replace(TokensFile.rfind(".jack"), 5, "T.xml");

    ofstream ofsT;

    ofsT.open(TokensFile);
    if(ofsT.fail()){
        cerr << "Cannot open file: " << TokensFile << "\n";
    }
    ofsT << "<tokens>\n";
    for(Token token : tokens){
        switch(token.tokenType){
            case TokenType::KEYWORD:
                if(!token.keyword.has_value()) cerr << static_cast<int>(token.tokenType) << " " << "1\n";
                ofsT << "<keyword> " << token.keyword.value() << " </keyword>\n";
                break;
            case TokenType::SYMBOL:
                if(!token.symbol.has_value()) cerr << static_cast<int>(token.tokenType) << " " << "2\n";
                if(token.symbol.value() == '<') ofsT << "<symbol> &lt; </symbol>\n";
                else if(token.symbol.value() == '>') ofsT << "<symbol> &gt; </symbol>\n";
                else if(token.symbol.value() == '&') ofsT << "<symbol> &amp; </symbol>\n";
                else ofsT << "<symbol> " << token.symbol.value() << " </symbol>\n";
                break;
            case TokenType::IDENTIFIER:
                if(!token.identifier.has_value()) cerr << static_cast<int>(token.tokenType) << " " << "3\n";
                ofsT << "<identifier> " << token.identifier.value() << " </identifier>\n";
                break;
            case TokenType::INT_CONST:
                if(!token.intVal.has_value()) cerr << static_cast<int>(token.tokenType) << " " << "4\n";
                ofsT << "<integerConstant> " << token.intVal.value() << " </integerConstant>\n";
                break;
            case TokenType::STRING_CONST:
                if(!token.stringVal.has_value()) cerr << static_cast<int>(token.tokenType) << " " << "5\n";
                ofsT << "<stringConstant> " << token.stringVal.value() << " </stringConstant>\n";
                break;
        }
    }
    ofsT << "</tokens>\n";
    ofsT.close();
}

void JackTokenizer::removeOneLineComments(){
    while(ifs.get() != '\n');
}

void JackTokenizer::removeSomeLinesComments(){
    while(ifs.get() != '*' || ifs.peek() != '/');
    ifs.get();
}

void JackTokenizer::tokenize(){
    bool isStringConst = false;
    char c;
    while(ifs.get(c)){
        if(isStringConst){
            if(c == '"'){
                isStringConst = false;
                if(!isStringConst){
                    addStringConstToken(tokenVal);
                    tokenVal.clear();
                }
            }
            else tokenVal.push_back(c);
        }
        else if(isspace(c)) continue;
        else if(c == '/'){
            if(ifs.peek() == '/') removeOneLineComments();
            else if(ifs.peek() == '*') removeSomeLinesComments();
            else{
                tokenVal.push_back(c);
                addSymbolToken(tokenVal);
                tokenVal.clear();
            }
        }
        else if(symbols.count(c)){
            tokenVal.push_back(c);
            addSymbolToken(tokenVal);
            tokenVal.clear();
        }
        else if(isdigit(c)){
            tokenVal.push_back(c);
            if(!isdigit(ifs.peek())){
                addIntConstToken(tokenVal);
                tokenVal.clear();
            }
        }
        else if(c == '"') isStringConst = true;
        else{
            tokenVal.push_back(c);

            if(isalnum(ifs.peek()) || ifs.peek() == '_') continue;

            else if(keywords.count(tokenVal)) addKeywordToken(tokenVal);
            else addIdentifierToken(tokenVal);
            tokenVal.clear();
        }
    }
}

void JackTokenizer::addKeywordToken(string tokenVal){
    tokens.push_back({TokenType::KEYWORD, tokenVal, nullopt, nullopt, nullopt, nullopt});
}

void JackTokenizer::addSymbolToken(string tokenVal){
    tokens.push_back({TokenType::SYMBOL, nullopt, tokenVal.front(), nullopt, nullopt, nullopt});
}

void JackTokenizer::addIdentifierToken(string tokenVal){
    tokens.push_back({TokenType::IDENTIFIER, nullopt, nullopt, tokenVal, nullopt, nullopt});
}

void JackTokenizer::addIntConstToken(string tokenVal){
    tokens.push_back({TokenType::INT_CONST, nullopt, nullopt, nullopt, stoi(tokenVal), nullopt});
}

void JackTokenizer::addStringConstToken(string tokenVal){
    tokens.push_back({TokenType::STRING_CONST, nullopt, nullopt, nullopt, nullopt, tokenVal});
}
