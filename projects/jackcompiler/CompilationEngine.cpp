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
    compileKeyword();
    // className
    compileIdentifier();
    // '{'
    compileSymbol();
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
    compileSymbol();

    ofs << "</class>\n";
}

void CompilationEngine::compileClassVarDec(){
    // classVarDec
    ofs << "<classVarDec>\n";

    // ('static' | 'field')
    compileKeyword();
    // type
    compileType();
    // varName
    compileIdentifier();
    // (',' varName)*
    while(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ','){
        // ','
        compileSymbol();
        // varName
        compileIdentifier();
    }
    // ';'
    compileSymbol();

    ofs << "</classVarDec>\n";
}

void CompilationEngine::compileSubroutine(){
    // subroutineDec
    ofs << "<subroutineDec>\n";

    // ('constructor' | 'function' | 'method')
    compileKeyword();
    // ('void' | type)
    if(tokenizer.tokenType() == TokenType::KEYWORD && tokenizer.keyword() == "void"){
        compileKeyword();
    }
    else if(isType()){
        compileType();
    }
    // subroutineName
    compileIdentifier();
    // '('
    compileSymbol();
    // parameterList
    compileParameterList();
    // ')'
    compileSymbol();
    // subroutineBody
    ofs << "<subroutineBody>\n";

    // '{'
    compileSymbol();
    // varDec*
    while(tokenizer.tokenType() == TokenType::KEYWORD && tokenizer.keyword() == "var"){
        // varDec
        compileVarDec();
    }
    // statements
    compileStatements();
    // '}'
    compileSymbol();

    ofs << "</subroutineBody>\n";

    ofs << "</subroutineDec>\n";
}

void CompilationEngine::compileParameterList(){
    ofs << "<parameterList>\n";

    // ((type varName) (',' type varName)*)?
    if(isType()){
        // type
        compileType();
        // varName
        compileIdentifier();
        // (',' type varName)*
        while(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ','){
            // ','
            compileSymbol();
            // type
            compileType();
            // varName
            compileIdentifier();
        }
    }

    ofs << "</parameterList>\n";
}

void CompilationEngine::compileVarDec(){
    ofs << "<varDec>\n";

    // 'var'
    compileKeyword();
    // type
    compileType();
    // varName
    compileIdentifier();
    // (',' varName)*
    while(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ','){
        // ','
        compileSymbol();
        // varName
        compileIdentifier();
    }
    // ';'
    compileSymbol();

    ofs << "</varDec>\n";
}

void CompilationEngine::compileStatements(){
    ofs << "<statements>\n";

    // statement*
    while(isStatement()){
        // statement
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

    ofs << "</statements>\n";
}

void CompilationEngine::compileLet(){
    ofs << "<letStatement>\n";

    // 'let'
    compileKeyword();
    // varName
    compileIdentifier();
    // ('[' expression ']')?
    if(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == '['){
        // '['
        compileSymbol();
        // expression
        compileExpression();
        // ']'
        compileSymbol();
    }
    // '='
    compileSymbol();
    // expression
    compileExpression();
    // ';'
    compileSymbol();

    ofs << "</letStatement>\n";
}

void CompilationEngine::compileIf(){
    ofs << "<ifStatement>\n";

    // 'if'
    compileKeyword();
    // '('
    compileSymbol();
    // expression
    compileExpression();
    // ')'
    compileSymbol();
    // '{'
    compileSymbol();
    // statements
    compileStatements();
    // '}'
    compileSymbol();
    // ('else' '{' statements '}')?
    if(tokenizer.tokenType() == TokenType::KEYWORD && tokenizer.keyword() == "else"){
        // 'else'
        compileKeyword();
        // '{'
        compileSymbol();
        // statements
        compileStatements();
        // '}'
        compileSymbol();
    }

    ofs << "</ifStatement>\n";
}

void CompilationEngine::compileWhile(){
    ofs << "<whileStatement>\n";

    // 'while'
    compileKeyword();
    // '('
    compileSymbol();
    // expression
    compileExpression();
    // ')'
    compileSymbol();
    // '{'
    compileSymbol();
    // statements
    compileStatements();
    // '}'
    compileSymbol();

    ofs << "</whileStatement>\n";
}

void CompilationEngine::compileDo(){
    ofs << "<doStatement>\n";

    // 'do'
    compileKeyword();
    // subroutineCall
    // subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'
    if(tokenizer.peekTokenType() == TokenType::SYMBOL || tokenizer.peekSymbol() == '.'){
        // (className | varName)
        compileIdentifier();
        // '.'
        compileSymbol();
    }
    // subroutineName
    compileIdentifier();
    // '('
    compileSymbol();
    // expressionList
    compileExpressionList();
    // ')'
    compileSymbol();
    // ';'
    compileSymbol();

    ofs << "</doStatement>\n";
}

void CompilationEngine::compileReturn(){
    ofs << "<returnStatement>\n";

    // 'return'
    compileKeyword();
    // expression?
    if(tokenizer.tokenType() != TokenType::SYMBOL || tokenizer.symbol() != ';'){
        compileExpression();
    }
    // ';'
    compileSymbol();

    ofs << "</returnStatement>\n";
}

void CompilationEngine::compileExpression(){
    ofs << "<expression>\n";

    // term
    compileTerm();
    // (op term)*
    while(isOp()){
        // op
        compileSymbol();
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
        if(tokenizer.tokenType() == TokenType::INT_CONST){
            ofs << "<integerConstant> " << tokenizer.intVal() << " </integerConstant>\n";
            if(tokenizer.hasMoreTokens()) tokenizer.advance();
        }
    }
    else if(tokenizer.tokenType() == TokenType::STRING_CONST){
        // stringConstant
        if(tokenizer.tokenType() == TokenType::STRING_CONST){
            ofs << "<stringConstant> " << tokenizer.stringVal() << " </stringConstant>\n";
            if(tokenizer.hasMoreTokens()) tokenizer.advance();
        }
    }
    else if(tokenizer.tokenType() == TokenType::KEYWORD && (tokenizer.keyword() == "true" || tokenizer.keyword() == "false" || tokenizer.keyword() == "null" || tokenizer.keyword() == "this")){
        // keywordConstant
        compileKeyword();
    }
    else if(tokenizer.tokenType() == TokenType::IDENTIFIER){
        // varName | varName '[' expression ']' | subroutineCall
        if(tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peekSymbol() == '['){
            // varName
            compileIdentifier();
            // '['
            compileSymbol();
            // expression
            compileExpression();
            // ']'
            compileSymbol();
        }
        else if(tokenizer.peekTokenType() == TokenType::SYMBOL && (tokenizer.peekSymbol() == '(' || tokenizer.peekSymbol() == '.')){
            // subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'
            if(tokenizer.peekTokenType() == TokenType::SYMBOL || tokenizer.peekSymbol() == '.'){
                // (className | varName)
                compileIdentifier();
                // '.'
                compileSymbol();
            }
            // subroutineName
            compileIdentifier();
            // '('
            compileSymbol();
            // expressionList
            compileExpressionList();
            // ')'
            compileSymbol();
        }
        else{
            // varName
            compileIdentifier();
        }
    }
    else if(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == '('){
        // '('
        compileSymbol();
        // expression
        compileExpression();
        // ')'
        compileSymbol();
    }
    else if(tokenizer.tokenType() == TokenType::SYMBOL && isUnaryOp()){
        // unaryOp
        compileSymbol();
        // term
        compileTerm();
    }

    ofs << "</term>\n";
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
            compileSymbol();
            // expression
            compileExpression();
        }
    }

    ofs << "</expressionList>\n";
}

void CompilationEngine::compileKeyword(){
    if(tokenizer.tokenType() == TokenType::KEYWORD){
        ofs << "<keyword> " << tokenizer.keyword() << " </keyword>\n";
        if(tokenizer.hasMoreTokens()) tokenizer.advance();
    }
}

void CompilationEngine::compileSymbol(){
    if(tokenizer.tokenType() == TokenType::SYMBOL){
        if(tokenizer.symbol() == '<') ofs << "<symbol> &lt; </symbol>\n";
        else if(tokenizer.symbol() == '>') ofs << "<symbol> &gt; </symbol>\n";
        else if(tokenizer.symbol() == '&') ofs << "<symbol> &amp; </symbol>\n";
        else ofs << "<symbol> " << tokenizer.symbol() << " </symbol>\n";
        if(tokenizer.hasMoreTokens()) tokenizer.advance();
    }
}

void CompilationEngine::compileIdentifier(){
    if(tokenizer.tokenType() == TokenType::IDENTIFIER){
        std::string idVal = tokenizer.identifier();
        ofs << "<identifier> " << idVal << " </identifier>\n";
        if(tokenizer.hasMoreTokens()) tokenizer.advance();
    }
}

void CompilationEngine::compileType(){
    if(isType()){
        if(tokenizer.tokenType() == TokenType::KEYWORD){
            compileKeyword();
        }
        if(tokenizer.tokenType() == TokenType::IDENTIFIER){
            compileIdentifier();
        }
    }
}

std::set<std::string> type = {"int", "char", "boolean"};
bool CompilationEngine::isType(){
    if(tokenizer.tokenType() == TokenType::KEYWORD){
        return (type.count(tokenizer.keyword()));
    }
    if(tokenizer.tokenType() == TokenType::IDENTIFIER){
        return true;
    }
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
