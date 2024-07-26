#include "CompilationEngine.hpp"

CompilationEngine::CompilationEngine(std::filesystem::path inputFile) : tokenizer(inputFile) {
    // 出力jackファイル
    std::string outputFile = inputFile.string();
    outputFile.replace(outputFile.rfind(".jack"), 5, ".xml");

    ofs.open(outputFile);
    if(ofs.fail()){
        std::cerr << "Cannot open file: " << outputFile << "\n";
    }
}

void CompilationEngine::compile(){
    compileClass();

    if(tokenizer.hasMoreTokens()){
        std::cerr << "Error: Compilation complete, but unresolved tokens remain.\n";
    }
}

void CompilationEngine::compileClass(){
    ofs << "<class>\n";

    // 'class'
    compileTerminal();
    // className
    compileTerminal();
    // '{'
    compileTerminal();
    // classVarDec*
    while(tokenizer.tokenType() == TokenType::KEYWORD && (tokenizer.keyword() == "static" || tokenizer.keyword() == "field")){
        // classVarDec
        compileClassVarDec();
    }
    // subroutineDec*
    while(tokenizer.tokenType() == TokenType::KEYWORD && (tokenizer.keyword() == "constructor" || tokenizer.keyword() == "function" || tokenizer.keyword() == "method")){
        // subroutineDec
        compileSubroutine();
    }
    // '}'
    compileTerminal();

    ofs << "</class>\n";
}

void CompilationEngine::compileClassVarDec(){
    // classVarDec
    ofs << "<classVarDec>\n";

    // ('static' | 'field')
    compileTerminal();
    // type
    compileTerminal();
    // varName
    compileTerminal();
    // (',' varName)*
    while(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ','){
        // ','
        compileTerminal();
        // varName
        compileTerminal();
    }
    // ';'
    compileTerminal();

    ofs << "</classVarDec>\n";
}

void CompilationEngine::compileSubroutine(){
    // subroutineDec
    ofs << "<subroutineDec>\n";

    // ('constructor' | 'function' | 'method')
    compileTerminal();
    // ('void' | type)
    compileTerminal();
    // subroutineName
    compileTerminal();
    // '('
    compileTerminal();
    // parameterList
    compileParameterList();
    // ')'
    compileTerminal();
    // subroutineBody
    compileSubroutineBody();

    ofs << "</subroutineDec>\n";
}

void CompilationEngine::compileParameterList(){
    ofs << "<parameterList>\n";

    // ((type varName) (',' type varName)*)?
    if(isType()){
        // type
        compileTerminal();
        // varName
        compileTerminal();
        // (',' type varName)*
        while(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ','){
            // ','
            compileTerminal();
            // type
            compileTerminal();
            // varName
            compileTerminal();
        }
    }

    ofs << "</parameterList>\n";
}

void CompilationEngine::compileSubroutineBody(){
    ofs << "<subroutineBody>\n";

    // '{'
    compileTerminal();
    // varDec*
    while(tokenizer.tokenType() == TokenType::KEYWORD && tokenizer.keyword() == "var"){
        // varDec
        compileVarDec();
    }
    // statements
    compileStatements();
    // '}'
    compileTerminal();

    ofs << "</subroutineBody>\n";
}

void CompilationEngine::compileVarDec(){
    ofs << "<varDec>\n";

    // 'var'
    compileTerminal();
    // type
    compileTerminal();
    // varName
    compileTerminal();
    // (',' varName)*
    while(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ','){
        // ','
        compileTerminal();
        // varName
        compileTerminal();
    }
    // ';'
    compileTerminal();

    ofs << "</varDec>\n";
}

void CompilationEngine::compileStatements(){
    ofs << "<statements>\n";

    // statement*
    while(isStatement()){
        // statement
        compileStatement();
    }

    ofs << "</statements>\n";
}

void CompilationEngine::compileStatement(){
    // (letStatement | ifStatement | whileStatement | doStatement | returnStatement)
    if(tokenizer.keyword() == "let"){
        // letStatement
        compileLet();
    }
    else if(tokenizer.keyword() == "if"){
        // ifStatement
        compileIf();
    }
    else if(tokenizer.keyword() == "while"){
        // whileStatement
        compileWhile();
    }
    else if(tokenizer.keyword() == "do"){
        // doStatement
        compileDo();
    }
    else{
        // returnStatement
        compileReturn();
    }
}

void CompilationEngine::compileLet(){
    ofs << "<letStatement>\n";

    // 'let'
    compileTerminal();
    // varName
    compileTerminal();
    // ('[' expression ']')?
    if(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == '['){
        // '['
        compileTerminal();
        // expression
        compileExpression();
        // ']'
        compileTerminal();
    }
    // '='
    compileTerminal();
    // expression
    compileExpression();
    // ';'
    compileTerminal();

    ofs << "</letStatement>\n";
}

void CompilationEngine::compileIf(){
    ofs << "<ifStatement>\n";

    // 'if'
    compileTerminal();
    // '('
    compileTerminal();
    // expression
    compileExpression();
    // ')'
    compileTerminal();
    // '{'
    compileTerminal();
    // statements
    compileStatements();
    // '}'
    compileTerminal();
    // ('else' '{' statements '}')?
    if(tokenizer.tokenType() == TokenType::KEYWORD && tokenizer.keyword() == "else"){
        // 'else'
        compileTerminal();
        // '{'
        compileTerminal();
        // statements
        compileStatements();
        // '}'
        compileTerminal();
    }

    ofs << "</ifStatement>\n";
}

void CompilationEngine::compileWhile(){
    ofs << "<whileStatement>\n";

    // 'while'
    compileTerminal();
    // '('
    compileTerminal();
    // expression
    compileExpression();
    // ')'
    compileTerminal();
    // '{'
    compileTerminal();
    // statements
    compileStatements();
    // '}'
    compileTerminal();

    ofs << "</whileStatement>\n";
}

void CompilationEngine::compileDo(){
    ofs << "<doStatement>\n";

    // 'do'
    compileTerminal();
    // subroutineCall
    compileSubroutineCall();
    // ';'
    compileTerminal();

    ofs << "</doStatement>\n";
}

void CompilationEngine::compileReturn(){
    ofs << "<returnStatement>\n";

    // 'return'
    compileTerminal();
    // expression?
    if(tokenizer.tokenType() != TokenType::SYMBOL || tokenizer.symbol() != ';'){
        compileExpression();
    }
    // ';'
    compileTerminal();

    ofs << "</returnStatement>\n";
}

void CompilationEngine::compileExpression(){
    ofs << "<expression>\n";

    // term
    compileTerm();
    // (op term)*
    while(isOp()){
        // op
        compileTerminal();
        // term
        compileTerm();
    }

    ofs << "</expression>\n";
}

void CompilationEngine::compileTerm(){
    ofs << "<term>\n";

    // integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' | subroutineCall | '(' expression ')' | unaryOp term
    if(tokenizer.tokenType() == TokenType::INT_CONST){
        // integerConstant
        compileTerminal();
    }
    else if(tokenizer.tokenType() == TokenType::STRING_CONST){
        // stringConstant
        compileTerminal();
    }
    else if(tokenizer.tokenType() == TokenType::KEYWORD && (tokenizer.keyword() == "true" || tokenizer.keyword() == "false" || tokenizer.keyword() == "null" || tokenizer.keyword() == "this")){
        // keywordConstant
        compileTerminal();
    }
    else if(tokenizer.tokenType() == TokenType::IDENTIFIER){
        // varName | varName '[' expression ']' | subroutineCall
        if(!tokenizer.hasMoreTokens()){
            // varName
            compileTerminal();
        }
        else if(tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peekSymbol() == '['){
            // varName
            compileTerminal();
            // '['
            compileTerminal();
            // expression
            compileExpression();
            // ']'
            compileTerminal();
        }
        else if(tokenizer.peekTokenType() == TokenType::SYMBOL && (tokenizer.peekSymbol() == '(' || tokenizer.peekSymbol() == '.')){
            // subroutineCall
            compileSubroutineCall();
        }
        else{
            // varName
            compileTerminal();
        }
    }
    else if(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == '('){
        // '('
        compileTerminal();
        // expression
        compileExpression();
        // ')'
        compileTerminal();
    }
    else if(tokenizer.tokenType() == TokenType::SYMBOL && isUnaryOp()){
        // unaryOp
        compileTerminal();
        // term
        compileTerm();
    }

    ofs << "</term>\n";
}

void CompilationEngine::compileSubroutineCall(){
    // subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'
    if(tokenizer.hasMoreTokens()){
        if(tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peekSymbol() == '('){
            // subroutineName
            compileTerminal();
            // '('
            compileTerminal();
            // expressionList
            compileExpressionList();
            // ')'
            compileTerminal();
        }
        else{
            // (className | varName)
            compileTerminal();
            // '.'
            compileTerminal();
            // subroutineName
            compileTerminal();
            // '('
            compileTerminal();
            // expressionList
            compileExpressionList();
            // ')'
            compileTerminal();
        }
    }
}

void CompilationEngine::compileExpressionList(){
    ofs << "<expressionList>\n";

    // (expression (',' expression)*)?
    if(tokenizer.tokenType() != TokenType::SYMBOL || (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == '(') || isUnaryOp()){
        // expression
        compileExpression();
        // (',' expression)*
        while(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ','){
            // ','
            compileTerminal();
            // expression
            compileExpression();
        }
    }

    ofs << "</expressionList>\n";
}

void CompilationEngine::compileTerminal(){
    switch(tokenizer.tokenType()){
        case TokenType::KEYWORD:
            ofs << "<keyword> " << tokenizer.keyword() << " </keyword>\n";
            break;
        case TokenType::SYMBOL:
            if(tokenizer.symbol() == '<') ofs << "<symbol> &lt; </symbol>\n";
            else if(tokenizer.symbol() == '>') ofs << "<symbol> &gt; </symbol>\n";
            else if(tokenizer.symbol() == '&') ofs << "<symbol> &amp; </symbol>\n";
            else ofs << "<symbol> " << tokenizer.symbol() << " </symbol>\n";
            break;
        case TokenType::INT_CONST:
            ofs << "<integerConstant> " << tokenizer.intVal() << " </integerConstant>\n";
            break;
        case TokenType::STRING_CONST:
            ofs << "<stringConstant> " << tokenizer.stringVal() << " </stringConstant>\n";
            break;
        case TokenType::IDENTIFIER:
            ofs << "<identifier> " << tokenizer.identifier() << " </identifier>\n";
            break;
    }

    if(tokenizer.hasMoreTokens()) tokenizer.advance();
}

std::set<std::string> type = {"int", "char", "boolean"};
bool CompilationEngine::isType(){
    if(tokenizer.tokenType() == TokenType::KEYWORD){
        return (type.count(tokenizer.keyword()));
    }
    if(tokenizer.tokenType() == TokenType::IDENTIFIER) return true;
    return false;
}

std::set<char> ops = {'+', '-', '*', '/', '&', '|', '<', '>', '='};
bool CompilationEngine::isOp(){
    return (tokenizer.tokenType() == TokenType::SYMBOL && ops.count(tokenizer.symbol()));
}

std::set<char> unaryOps = {'-', '~'};
bool CompilationEngine::isUnaryOp(){
    return (tokenizer.tokenType() == TokenType::SYMBOL && unaryOps.count(tokenizer.symbol()));
}

std::set<std::string> statementKeywords = {"let", "if", "while", "do", "return"};
bool CompilationEngine::isStatement(){
    return (tokenizer.tokenType() == TokenType::KEYWORD && statementKeywords.count(tokenizer.keyword()));
}
