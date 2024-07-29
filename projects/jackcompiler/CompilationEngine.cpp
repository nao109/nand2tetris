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
    if(tokenizer.tokenType() == TokenType::IDENTIFIER){
        className = tokenizer.identifier();
    }
    compileClassName();
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
    Kind kind;
    std::string type;

    // classVarDec
    ofs << "<classVarDec>\n";

    // ('static' | 'field')
    kind = compileKind();
    // type
    type = compileType();
    // varName
    compileDec(type, kind);
    // (',' varName)*
    while(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ','){
        // ','
        compileSymbol();
        // varName
        compileDec(type, kind);
    }
    // ';'
    compileSymbol();

    ofs << "</classVarDec>\n";
}

void CompilationEngine::compileSubroutine(){
    symbolTable.startSubroutine();

    // subroutineDec
    ofs << "<subroutineDec>\n";

    // ('constructor' | 'function' | 'method')
    if(tokenizer.tokenType() == TokenType::KEYWORD && tokenizer.keyword() == "method"){
        symbolTable.define("this", className, Kind::ARG);
    }
    compileKeyword();
    // ('void' | type)
    if(tokenizer.tokenType() == TokenType::KEYWORD && tokenizer.keyword() == "void"){
        compileKeyword();
    }
    else if(isType()){
        compileType();
    }
    // subroutineName
    compileSubroutineName();
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
    std::string type;

    ofs << "<parameterList>\n";

    // ((type varName) (',' type varName)*)?
    if(isType()){
        // type
        type = compileType();
        // varName
        compileDec(type, Kind::ARG);
        // (',' type varName)*
        while(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ','){
            // ','
            compileSymbol();
            // type
            type = compileType();
            // varName
            compileDec(type, Kind::ARG);
        }
    }

    ofs << "</parameterList>\n";
}

void CompilationEngine::compileVarDec(){
    std::string type;

    ofs << "<varDec>\n";

    // 'var'
    compileKeyword();
    // type
    type = compileType();
    // varName
    compileDec(type, Kind::VAR);
    // (',' varName)*
    while(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ','){
        // ','
        compileSymbol();
        // varName
        compileDec(type, Kind::VAR);
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
    if(tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peekSymbol() == '.'){
        // (className | varName)
        if(tokenizer.tokenType() == TokenType::IDENTIFIER){
            if(symbolTable.kindOf(tokenizer.identifier()) == Kind::NONE) compileClassName();
            else compileIdentifier();
        }
        // '.'
        compileSymbol();
    }
    // subroutineName
    compileSubroutineName();
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
            if(tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peekSymbol() == '.'){
                // (className | varName)
                if(tokenizer.tokenType() == TokenType::IDENTIFIER){
                    if(symbolTable.kindOf(tokenizer.identifier()) == Kind::NONE) compileClassName();
                    else compileIdentifier();
                }
                // '.'
                compileSymbol();
            }
            // subroutineName
            compileSubroutineName();
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

std::string CompilationEngine::compileIdentifier(){
    if(tokenizer.tokenType() == TokenType::IDENTIFIER){
        std::string idVal = tokenizer.identifier();
        Kind kind = symbolTable.kindOf(idVal);
        int index = symbolTable.indexOf(idVal);
        ofs << "<identifier>\n";
        ofs << "<value> " << idVal << " <value>\n";
        ofs << "<category> " << kind2string(kind) << " <category>\n";
        ofs << "<defined> used <defined>\n";
        ofs << "<kind> " << kind2string(kind) << " </kind>\n";
        ofs << "<index> " << index << " </index>\n";
        ofs << "</identifier>\n";
        if(tokenizer.hasMoreTokens()) tokenizer.advance();
        return idVal;
    }
    return std::string();
}

void CompilationEngine::compileClassName(){
    if(tokenizer.tokenType() == TokenType::IDENTIFIER){
        std::string idVal = tokenizer.identifier();
        ofs << "<identifier>\n";
        ofs << "<value> " << idVal << " <value>\n";
        ofs << "<category> class <category>\n";
        ofs << "</identifier>\n";
        if(tokenizer.hasMoreTokens()) tokenizer.advance();
    }
}

void CompilationEngine::compileSubroutineName(){
    if(tokenizer.tokenType() == TokenType::IDENTIFIER){
        std::string idVal = tokenizer.identifier();
        ofs << "<identifier>\n";
        ofs << "<value> " << idVal << " <value>\n";
        ofs << "<category> subroutine <category>\n";
        ofs << "</identifier>\n";
        if(tokenizer.hasMoreTokens()) tokenizer.advance();
    }
}

void CompilationEngine::compileDec(std::string type, Kind kind){
    // varName
    if(tokenizer.tokenType() == TokenType::IDENTIFIER){
        std::string varName = tokenizer.identifier();
        ofs << "<identifier>\n";
        ofs << "<value> " << varName << " <value>\n";
        ofs << "<category> " << kind2string(kind) << " </category>\n";
        ofs << "<defined> defined <defined>\n";
        if(kind != Kind::NONE){
            ofs << "<kind> " << kind2string(kind) << " </kind>\n";
            ofs << "<index> " << symbolTable.varCount(kind) << " </index>\n";
        }
        ofs << "</identifier>\n";
        symbolTable.define(varName, type, kind);
        if(tokenizer.hasMoreTokens()) tokenizer.advance();
    }
}

std::string CompilationEngine::compileType(){
    if(isType()){
        std::string type;

        if(tokenizer.tokenType() == TokenType::KEYWORD){
            type = tokenizer.keyword();
            compileKeyword();
            return type;
        }
        if(tokenizer.tokenType() == TokenType::IDENTIFIER){
            type = tokenizer.identifier();
            compileClassName();
            return type;
        }
    }
    return std::string();
}

Kind CompilationEngine::compileKind(){
    if(tokenizer.tokenType() == TokenType::KEYWORD){
        if(tokenizer.keyword() == "static"){
            compileKeyword();
            return Kind::STATIC;
        }
        if(tokenizer.keyword() == "field"){
            compileKeyword();
            return Kind::FIELD;
        }
    }
    return Kind::NONE;
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
