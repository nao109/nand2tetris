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
    this->className = compileClassName();
    // '{'
    compileSymbol();
    // classVarDec*
    while(consume(TokenType::KEYWORD, "static") || consume(TokenType::KEYWORD, "field")){
        // classVarDec
        compileClassVarDec();
    }
    // subroutineDec*
    while(consume(TokenType::KEYWORD, "constructor") || consume(TokenType::KEYWORD, "function") || consume(TokenType::KEYWORD, "method")){
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
    while(consume(TokenType::SYMBOL, ",")){
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
    std::string subroutineKind = compileKeyword();
    if(subroutineKind == "method"){
        symbolTable.define("this", className, Kind::ARG);
    }
    // ('void' | type)
    if(consume(TokenType::KEYWORD, "void")){
        compileKeyword();
    }
    else if(isType()){
        compileType();
    }
    // subroutineName
    this->subroutineName = compileSubroutineName();
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
    while(consume(TokenType::KEYWORD, "var")){
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
        std::string type = compileType();
        // varName
        compileDec(type, Kind::ARG);
        // (',' type varName)*
        while(consume(TokenType::SYMBOL, ",")){
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
    ofs << "<varDec>\n";

    // 'var'
    Kind kind = compileKind();
    // type
    std::string type = compileType();
    // varName
    compileDec(type, kind);
    // (',' varName)*
    while(consume(TokenType::SYMBOL, ",")){
        // ','
        compileSymbol();
        // varName
        compileDec(type, kind);
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
    if(consume(TokenType::SYMBOL, "[")){
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
    if(consume(TokenType::KEYWORD, "else")){
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
    compileSubroutineCall();
    // ';'
    compileSymbol();

    ofs << "</doStatement>\n";
}

void CompilationEngine::compileReturn(){
    ofs << "<returnStatement>\n";

    // 'return'
    compileKeyword();
    // expression?
    if(!consume(TokenType::SYMBOL, ";")){
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

    if(consumeType(TokenType::INT_CONST)){
        // integerConstant
        ofs << "<integerConstant> " << tokenizer.intVal() << " </integerConstant>\n";
        if(tokenizer.hasMoreTokens()) tokenizer.advance();
    }
    else if(consumeType(TokenType::STRING_CONST)){
        // stringConstant
        ofs << "<stringConstant> " << tokenizer.stringVal() << " </stringConstant>\n";
        if(tokenizer.hasMoreTokens()) tokenizer.advance();
    }
    else if(consume(TokenType::KEYWORD, "true") || consume(TokenType::KEYWORD, "false") || consume(TokenType::KEYWORD, "null") || consume(TokenType::KEYWORD, "this")){
        // keywordConstant
        compileKeyword();
    }
    else if(consumeType(TokenType::IDENTIFIER)){
        // varName | varName '[' expression ']' | subroutineCall
        if(tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peekSymbol() == "["){
            // varName
            compileIdentifier();
            // '['
            compileSymbol();
            // expression
            compileExpression();
            // ']'
            compileSymbol();
        }
        else if(tokenizer.peekTokenType() == TokenType::SYMBOL && (tokenizer.peekSymbol() == "(" || tokenizer.peekSymbol() == ".")){
            // subroutineCall
            compileSubroutineCall();
        }
        else{
            // varName
            compileIdentifier();
        }
    }
    else if(consume(TokenType::SYMBOL, "(")){
        // '('
        compileSymbol();
        // expression
        compileExpression();
        // ')'
        compileSymbol();
    }
    else if(isUnaryOp()){
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
    if(!consumeType(TokenType::SYMBOL) || consume(TokenType::SYMBOL, "(") || isUnaryOp()){
        // expression
        compileExpression();
        // (',' expression)*
        while(consume(TokenType::SYMBOL, ",")){
            // ','
            compileSymbol();
            // expression
            compileExpression();
        }
    }

    ofs << "</expressionList>\n";
}

void CompilationEngine::compileSubroutineCall(){
    // subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'
    if(tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peekSymbol() == "."){
        if(consumeType(TokenType::IDENTIFIER)){
            if(symbolTable.kindOf(tokenizer.identifier()) == Kind::NONE){
                // className
                compileClassName();
            }
            else{
                // varName
                compileIdentifier();
            }
            // '.'
            compileSymbol();
            // subroutineName
            compileSubroutineName();
            // '('
            compileSymbol();
            // expressionList
            compileExpressionList();
            // ')'
            compileSymbol();
        }
    }
    else{
        // subroutineName
        compileSubroutineName();
        // '('
        compileSymbol();
        // expressionList
        compileExpressionList();
        // ')'
        compileSymbol();
    }
}

std::string CompilationEngine::compileKeyword(){
    if(consumeType(TokenType::KEYWORD)){
        std::string keyword = tokenizer.keyword();
        ofs << "<keyword> " << keyword << " </keyword>\n";
        if(tokenizer.hasMoreTokens()) tokenizer.advance();
        return keyword;
    }
    return std::string();
}

std::string CompilationEngine::compileSymbol(){
    if(consumeType(TokenType::SYMBOL)){
        std::string symbol = tokenizer.symbol();
        if(tokenizer.symbol() == "<") ofs << "<symbol> &lt; </symbol>\n";
        else if(tokenizer.symbol() == ">") ofs << "<symbol> &gt; </symbol>\n";
        else if(tokenizer.symbol() == "&") ofs << "<symbol> &amp; </symbol>\n";
        else ofs << "<symbol> " << symbol << " </symbol>\n";
        if(tokenizer.hasMoreTokens()) tokenizer.advance();
        return symbol;
    }
    return std::string();
}

std::string CompilationEngine::compileIdentifier(){
    if(consumeType(TokenType::IDENTIFIER)){
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

std::string CompilationEngine::compileClassName(){
    if(consumeType(TokenType::IDENTIFIER)){
        std::string idVal = tokenizer.identifier();
        ofs << "<identifier>\n";
        ofs << "<value> " << idVal << " <value>\n";
        ofs << "<category> class <category>\n";
        ofs << "</identifier>\n";
        if(tokenizer.hasMoreTokens()) tokenizer.advance();
        return idVal;
    }
    return std::string();
}

std::string CompilationEngine::compileSubroutineName(){
    if(consumeType(TokenType::IDENTIFIER)){
        std::string idVal = tokenizer.identifier();
        ofs << "<identifier>\n";
        ofs << "<value> " << idVal << " <value>\n";
        ofs << "<category> subroutine <category>\n";
        ofs << "</identifier>\n";
        if(tokenizer.hasMoreTokens()) tokenizer.advance();
        return idVal;
    }
    return std::string();
}

void CompilationEngine::compileDec(std::string type, Kind kind){
    // varName
    if(consumeType(TokenType::IDENTIFIER)){
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

        if(consumeType(TokenType::KEYWORD)){
            type = tokenizer.keyword();
            compileKeyword();
            return type;
        }
        if(consumeType(TokenType::IDENTIFIER)){
            type = tokenizer.identifier();
            compileClassName();
            return type;
        }
    }
    return std::string();
}

Kind CompilationEngine::compileKind(){
    if(consumeType(TokenType::KEYWORD)){
        std::string keyVal = compileKeyword();
        if(keyVal == "static") return Kind::STATIC;
        if(keyVal == "field") return Kind::FIELD;
        if(keyVal == "var") return Kind::VAR;
    }
    return Kind::NONE;
}

std::set<std::string> type = {"int", "char", "boolean"};
bool CompilationEngine::isType(){
    if(consumeType(TokenType::KEYWORD)){
        return (type.count(tokenizer.keyword()));
    }
    if(consumeType(TokenType::IDENTIFIER)){
        return true;
    }
    return false;
}

bool CompilationEngine::consumeType(TokenType tokenType){
    return (tokenizer.tokenType() == tokenType);
}

bool CompilationEngine::consume(TokenType tokenType, std::string str){
    if(tokenType == TokenType::KEYWORD){
        return (consumeType(TokenType::KEYWORD) && tokenizer.keyword() == str);
    }
    if(tokenType == TokenType::SYMBOL){
        return (consumeType(TokenType::SYMBOL) && tokenizer.symbol() == str);
    }
    if(tokenType == TokenType::IDENTIFIER){
        return (consumeType(TokenType::IDENTIFIER) && tokenizer.identifier() == str);
    }
    return false;
}

std::set<std::string> ops = {"+", "-", "*", "/", "&", "|", "<", ">", "="};
bool CompilationEngine::isOp(){
    return (consumeType(TokenType::SYMBOL) && ops.count(tokenizer.symbol()));
}

std::set<std::string> unaryOps = {"-", "~"};
bool CompilationEngine::isUnaryOp(){
    return (consumeType(TokenType::SYMBOL) && unaryOps.count(tokenizer.symbol()));
}

std::set<std::string> statementKeywords = {"let", "if", "while", "do", "return"};
bool CompilationEngine::isStatement(){
    return (consumeType(TokenType::KEYWORD) && statementKeywords.count(tokenizer.keyword()));
}
