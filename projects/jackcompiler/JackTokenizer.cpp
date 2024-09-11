#include "JackTokenizer.hpp"

std::set<std::string> keywords = {
    "class", "method", "function", "constructor", "int", "boolean", "char", "void", "var","static",
    "field", "let", "do", "if", "else", "while", "return", "true", "false", "null", "this"
};
std::set<char> symbols = {'{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~'};

JackTokenizer::JackTokenizer(std::filesystem::path inputFile) {
    // 入力jackファイル
    ifs.open(inputFile);
    if(ifs.fail()){
        std::cerr << "Cannot open file: " << inputFile.string() << "\n";
    }

    // 初期化
    tokenVal = "";
    tokens.clear();
    id = 0;

    tokenize();
    printTokens(inputFile);
}

bool JackTokenizer::hasMoreTokens(){
    return id + 1 < tokens.size();
}

void JackTokenizer::advance(){
    ++id;
}

TokenType JackTokenizer::tokenType(){
    return tokens[id].tokenType;
}

std::string JackTokenizer::keyword(){
    return tokens[id].keyword.value();
}

std::string JackTokenizer::symbol(){
    return tokens[id].symbol.value();
}

std::string JackTokenizer::identifier(){
    return tokens[id].identifier.value();
}

int JackTokenizer::intVal(){
    return tokens[id].intVal.value();
}

std::string JackTokenizer::stringVal(){
    return tokens[id].stringVal.value();
}

TokenType JackTokenizer::peekTokenType(){
    return tokens[id + 1].tokenType;
}

std::string JackTokenizer::peekSymbol(){
    return tokens[id + 1].symbol.value();
}

void JackTokenizer::printTokens(std::filesystem::path inputFile){
    std::string TokensFile = inputFile.string();
    TokensFile.replace(TokensFile.rfind(".jack"), 5, "T.xml");

    std::ofstream ofsT;

    ofsT.open(TokensFile);
    if(ofsT.fail()){
        std::cerr << "Cannot open file: " << TokensFile << "\n";
    }
    ofsT << "<tokens>\n";
    for(Token token : tokens){
        switch(token.tokenType){
            case TokenType::KEYWORD:
                if(!token.keyword.has_value()) std::cerr << static_cast<int>(token.tokenType) << " " << "1\n";
                ofsT << "<keyword> " << token.keyword.value() << " </keyword>\n";
                break;
            case TokenType::SYMBOL:
                if(!token.symbol.has_value()) std::cerr << static_cast<int>(token.tokenType) << " " << "2\n";
                if(token.symbol.value() == "<") ofsT << "<symbol> &lt; </symbol>\n";
                else if(token.symbol.value() == ">") ofsT << "<symbol> &gt; </symbol>\n";
                else if(token.symbol.value() == "&") ofsT << "<symbol> &amp; </symbol>\n";
                else ofsT << "<symbol> " << token.symbol.value() << " </symbol>\n";
                break;
            case TokenType::IDENTIFIER:
                if(!token.identifier.has_value()) std::cerr << static_cast<int>(token.tokenType) << " " << "3\n";
                ofsT << "<identifier> " << token.identifier.value() << " </identifier>\n";
                break;
            case TokenType::INT_CONST:
                if(!token.intVal.has_value()) std::cerr << static_cast<int>(token.tokenType) << " " << "4\n";
                ofsT << "<integerConstant> " << token.intVal.value() << " </integerConstant>\n";
                break;
            case TokenType::STRING_CONST:
                if(!token.stringVal.has_value()) std::cerr << static_cast<int>(token.tokenType) << " " << "5\n";
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
    char c;
    while(ifs.get(c)){
        if(c == '"') {
            while(ifs.peek() != '"'){
                ifs.get(c);
                tokenVal.push_back(c);
            }
            ifs.get(c);
            addStringConstToken(tokenVal);
        }
        else if(isspace(c)) continue;
        else if(c == '/'){
            if(ifs.peek() == '/') removeOneLineComments();
            else if(ifs.peek() == '*') removeSomeLinesComments();
            else{
                tokenVal.push_back(c);
                addSymbolToken(tokenVal);
            }
        }
        else if(symbols.count(c)){
            tokenVal.push_back(c);
            addSymbolToken(tokenVal);
        }
        else if(isdigit(c)){
            tokenVal.push_back(c);
            while(isdigit(ifs.peek())){
                ifs.get(c);
                tokenVal.push_back(c);
            }
            addIntConstToken(tokenVal);
        }
        else if(isalpha(c) || c == '_'){
            tokenVal.push_back(c);
            while(isalnum(ifs.peek()) || ifs.peek() == '_'){
                ifs.get(c);
                tokenVal.push_back(c);
            }
            if(keywords.count(tokenVal)) addKeywordToken(tokenVal);
            else addIdentifierToken(tokenVal);
        }
        tokenVal.clear();
    }
}

void JackTokenizer::addKeywordToken(std::string tokenVal){
    tokens.push_back({TokenType::KEYWORD, tokenVal, std::nullopt, std::nullopt, std::nullopt, std::nullopt});
}

void JackTokenizer::addSymbolToken(std::string tokenVal){
    tokens.push_back({TokenType::SYMBOL, std::nullopt, tokenVal, std::nullopt, std::nullopt, std::nullopt});
}

void JackTokenizer::addIdentifierToken(std::string tokenVal){
    tokens.push_back({TokenType::IDENTIFIER, std::nullopt, std::nullopt, tokenVal, std::nullopt, std::nullopt});
}

void JackTokenizer::addIntConstToken(std::string tokenVal){
    tokens.push_back({TokenType::INT_CONST, std::nullopt, std::nullopt, std::nullopt, stoi(tokenVal), std::nullopt});
}

void JackTokenizer::addStringConstToken(std::string tokenVal){
    tokens.push_back({TokenType::STRING_CONST, std::nullopt, std::nullopt, std::nullopt, std::nullopt, tokenVal});
}
