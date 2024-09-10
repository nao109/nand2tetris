#include "CompilationEngine.hpp"

CompilationEngine::CompilationEngine(std::filesystem::path inputFile) : tokenizer(inputFile), vmwriter(inputFile) {
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
    initLabel();

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

    // write VM
    vmwriter.writeFunction(this->className + "." + this->subroutineName, symbolTable.varCount(Kind::VAR));
    if(subroutineKind == "constructor"){
        vmwriter.writePush(Segment::CONST, symbolTable.varCount(Kind::FIELD));
        vmwriter.writeCall("Memory.alloc", 1);
        vmwriter.writePop(Segment::POINTER, 0);
    }
    else if(subroutineKind == "method"){
        vmwriter.writePush(Segment::ARG, 0);
        vmwriter.writePop(Segment::POINTER, 0);
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
    std::string idVal = compileIdentifier();
    // ('[' expression ']')?
    if(consume(TokenType::SYMBOL, "[")){
        // '['
        compileSymbol();
        // expression
        compileExpression();
        // ']'
        compileSymbol();

        // // write VM
        switch(symbolTable.kindOf(idVal)){
            case Kind::VAR:
                vmwriter.writePush(Segment::LOCAL, symbolTable.indexOf(idVal));
                break;
            case Kind::ARG:
                vmwriter.writePush(Segment::ARG, symbolTable.indexOf(idVal));
                break;
            case Kind::FIELD:
                vmwriter.writePush(Segment::THIS, symbolTable.indexOf(idVal));
                break;
            case Kind::STATIC:
                vmwriter.writePush(Segment::STATIC, symbolTable.indexOf(idVal));
                break;
            default:
                break;
        }
        vmwriter.writeArithmetic(Command::ADD);

        // '='
        compileSymbol();
        // expression
        compileExpression();
        // ';'
        compileSymbol();

        // write VM
        vmwriter.writePop(Segment::TEMP, 0);
        vmwriter.writePop(Segment::POINTER, 1);
        vmwriter.writePush(Segment::TEMP, 0);
        vmwriter.writePop(Segment::THAT, 0);
    }
    else{
        // '='
        compileSymbol();
        // expression
        compileExpression();
        // ';'
        compileSymbol();

        // write VM
        switch(symbolTable.kindOf(idVal)){
            case Kind::VAR:
                vmwriter.writePop(Segment::LOCAL, symbolTable.indexOf(idVal));
                break;
            case Kind::ARG:
                vmwriter.writePop(Segment::ARG, symbolTable.indexOf(idVal));
                break;
            case Kind::FIELD:
                vmwriter.writePop(Segment::THIS, symbolTable.indexOf(idVal));
                break;
            case Kind::STATIC:
                vmwriter.writePop(Segment::STATIC, symbolTable.indexOf(idVal));
                break;
            default:
                break;
        }
    }

    ofs << "</letStatement>\n";
}

void CompilationEngine::compileIf(){
    ofs << "<ifStatement>\n";

    std::string label_str = newIfLabel();
    std::string if_true = "IF_TRUE" + label_str;
    std::string if_false = "IF_FALSE" + label_str;
    std::string if_end = "IF_END" + label_str;

    // 'if'
    compileKeyword();
    // '('
    compileSymbol();
    // expression
    compileExpression();
    // ')'

    // write VM
    vmwriter.writeIf(if_true);
    vmwriter.writeGoto(if_false);
    vmwriter.writeLabel(if_true);

    compileSymbol();
    // '{'
    compileSymbol();
    // statements
    compileStatements();
    // '}'
    compileSymbol();
    // ('else' '{' statements '}')?
    if(consume(TokenType::KEYWORD, "else")){
        // write VM
        vmwriter.writeGoto(if_end);
        vmwriter.writeLabel(if_false);

        // 'else'
        compileKeyword();
        // '{'
        compileSymbol();
        // statements
        compileStatements();
        // '}'
        compileSymbol();

        // write VM
        vmwriter.writeLabel(if_end);
    }
    else{
        // write VM
        vmwriter.writeLabel(if_false);
    }

    ofs << "</ifStatement>\n";
}

void CompilationEngine::compileWhile(){
    ofs << "<whileStatement>\n";

    std::string label_str = newWhileLabel();
    std::string while_exp = "WHILE_EXP" + label_str;
    std::string while_end = "WHILE_END" + label_str;

    // write VM
    vmwriter.writeLabel(while_exp);

    // 'while'
    compileKeyword();
    // '('
    compileSymbol();
    // expression
    compileExpression();
    // ')'

    // write VM
    vmwriter.writeArithmetic(Command::NOT);
    vmwriter.writeIf(while_end);

    compileSymbol();
    // '{'
    compileSymbol();
    // statements
    compileStatements();
    // '}'
    compileSymbol();

    // write VM
    vmwriter.writeGoto(while_exp);
    vmwriter.writeLabel(while_end);

    ofs << "</whileStatement>\n";
}

void CompilationEngine::compileDo(){
    ofs << "<doStatement>\n";

    // 'do'
    compileKeyword();
    // subroutineCall
    compileSubroutineCall();

    // write VM
    vmwriter.writePop(Segment::TEMP, 0);

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
    else{
        // write VM
        vmwriter.writePush(Segment::CONST, 0);
    }
    // ';'
    compileSymbol();

    // write VM
    vmwriter.writeReturn();

    ofs << "</returnStatement>\n";
}

void CompilationEngine::compileExpression(){
    ofs << "<expression>\n";

    // term
    compileTerm();
    // (op term)*
    while(isOp()){
        // op
        std::string op = compileSymbol();
        // term
        compileTerm();

        // write VM
        Command command = op2command(op);
        if(command != Command::NONE) vmwriter.writeArithmetic(command);
        else if(op == "*") vmwriter.writeCall("Math.multiply", 2);
        else if(op == "/") vmwriter.writeCall("Math.divide", 2);
    }

    ofs << "</expression>\n";
}

void CompilationEngine::compileTerm(){
    ofs << "<term>\n";

    // integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' | subroutineCall | '(' expression ')' | unaryOp term

    if(consumeType(TokenType::INT_CONST)){
        // integerConstant
        ofs << "<integerConstant> " << tokenizer.intVal() << " </integerConstant>\n";
        int intVal = tokenizer.intVal();
        if(tokenizer.hasMoreTokens()) tokenizer.advance();

        // writeVM
        vmwriter.writePush(Segment::CONST, intVal);
    }
    else if(consumeType(TokenType::STRING_CONST)){
        // stringConstant
        ofs << "<stringConstant> " << tokenizer.stringVal() << " </stringConstant>\n";
        std::string str = tokenizer.stringVal();
        if(tokenizer.hasMoreTokens()) tokenizer.advance();

        // write VM
        vmwriter.writePush(Segment::CONST, (int)str.size());
        vmwriter.writeCall("String.new", 1);
        for(char c : str){
            vmwriter.writePush(Segment::CONST, (int)c);
            vmwriter.writeCall("String.appendChar", 2);
        }
    }
    else if(consume(TokenType::KEYWORD, "true") || consume(TokenType::KEYWORD, "false") || consume(TokenType::KEYWORD, "null") || consume(TokenType::KEYWORD, "this")){
        // keywordConstant
        std::string keyVal = compileKeyword();

        // write VM
        if(keyVal == "true"){
            vmwriter.writePush(Segment::CONST, 0);
            vmwriter.writeArithmetic(Command::NOT);
        }
        else if(keyVal == "false" || keyVal == "null"){
            vmwriter.writePush(Segment::CONST, 0);
        }
        else if(keyVal == "this"){
            vmwriter.writePush(Segment::POINTER, 0);
        }
    }
    else if(consumeType(TokenType::IDENTIFIER)){
        // varName | varName '[' expression ']' | subroutineCall
        if(tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peekSymbol() == "["){
            // varName
            std::string idVal = compileIdentifier();
            // '['
            compileSymbol();
            // expression
            compileExpression();
            // ']'
            compileSymbol();

            // write VM
            switch(symbolTable.kindOf(idVal)){
                case Kind::VAR:
                    vmwriter.writePush(Segment::LOCAL, symbolTable.indexOf(idVal));
                    break;
                case Kind::ARG:
                    vmwriter.writePush(Segment::ARG, symbolTable.indexOf(idVal));
                    break;
                case Kind::FIELD:
                    vmwriter.writePush(Segment::THIS, symbolTable.indexOf(idVal));
                    break;
                case Kind::STATIC:
                    vmwriter.writePush(Segment::STATIC, symbolTable.indexOf(idVal));
                    break;
                default:
                    break;
            }
            vmwriter.writeArithmetic(Command::ADD);
            vmwriter.writePop(Segment::POINTER, 1);
            vmwriter.writePush(Segment::THAT, 0);
        }
        else if(tokenizer.peekTokenType() == TokenType::SYMBOL && (tokenizer.peekSymbol() == "(" || tokenizer.peekSymbol() == ".")){
            // subroutineCall
            compileSubroutineCall();
        }
        else{
            // varName
            std::string idVal = compileIdentifier();

            // write VM
            switch(symbolTable.kindOf(idVal)){
                case Kind::VAR:
                    vmwriter.writePush(Segment::LOCAL, symbolTable.indexOf(idVal));
                    break;
                case Kind::ARG:
                    vmwriter.writePush(Segment::ARG, symbolTable.indexOf(idVal));
                    break;
                case Kind::FIELD:
                    vmwriter.writePush(Segment::THIS, symbolTable.indexOf(idVal));
                    break;
                case Kind::STATIC:
                    vmwriter.writePush(Segment::STATIC, symbolTable.indexOf(idVal));
                    break;
                default:
                    break;
            }
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
        std::string unaryOp = compileSymbol();
        // term
        compileTerm();

        // write VM
        Command command = unaryop2command(unaryOp);
        if(command != Command::NONE) vmwriter.writeArithmetic(command);
    }

    ofs << "</term>\n";
}

int CompilationEngine::compileExpressionList(){
    ofs << "<expressionList>\n";

    int nExpressions = 0;

    // (expression (',' expression)*)?
    if(!consumeType(TokenType::SYMBOL) || consume(TokenType::SYMBOL, "(") || isUnaryOp()){
        // expression
        compileExpression();
        nExpressions++;
        // (',' expression)*
        while(consume(TokenType::SYMBOL, ",")){
            // ','
            compileSymbol();
            // expression
            compileExpression();
            nExpressions++;
        }
    }

    ofs << "</expressionList>\n";

    return nExpressions;
}

void CompilationEngine::compileSubroutineCall(){
    // subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'
    if(tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peekSymbol() == "."){
        if(consumeType(TokenType::IDENTIFIER)){
            if(symbolTable.kindOf(tokenizer.identifier()) == Kind::NONE){
                // className
                std::string idVal = compileClassName();
                // '.'
                compileSymbol();
                // subroutineName
                std::string subroutineName = compileSubroutineName();
                // '('
                compileSymbol();
                // expressionList
                int nExpressions = compileExpressionList();
                // ')'
                compileSymbol();

                // write VM
                vmwriter.writeCall(idVal + "." + subroutineName, nExpressions);
            }
            else{
                // varName
                std::string idVal = compileIdentifier();
                // '.'
                compileSymbol();
                // subroutineName
                std::string subroutineName = compileSubroutineName();

                // write VM
                switch(symbolTable.kindOf(idVal)){
                    case Kind::VAR:
                        vmwriter.writePush(Segment::LOCAL, symbolTable.indexOf(idVal));
                        break;
                    case Kind::ARG:
                        vmwriter.writePush(Segment::ARG, symbolTable.indexOf(idVal));
                        break;
                    case Kind::FIELD:
                        vmwriter.writePush(Segment::THIS, symbolTable.indexOf(idVal));
                        break;
                    case Kind::STATIC:
                        vmwriter.writePush(Segment::STATIC, symbolTable.indexOf(idVal));
                        break;
                    default:
                        break;
                }

                // '('
                compileSymbol();
                // expressionList
                int nExpressions = compileExpressionList();
                // ')'
                compileSymbol();

                vmwriter.writeCall(symbolTable.typeOf(idVal) + "." + subroutineName, nExpressions + 1);
            }
        }
    }
    else{    
        // subroutineName
        std::string subroutineName = compileSubroutineName();

        // write VM
        vmwriter.writePush(Segment::POINTER, 0);

        // '('
        compileSymbol();
        // expressionList
        int nExpressions = compileExpressionList();
        // ')'
        compileSymbol();

        // write VM
        vmwriter.writeCall(this->className + "." + subroutineName, nExpressions + 1);
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

void CompilationEngine::initLabel(){
    ifLabel = 0;
    whileLabel = 0;
}

std::string CompilationEngine::newIfLabel(){
    return std::to_string(ifLabel++);
}

std::string CompilationEngine::newWhileLabel(){
    return std::to_string(whileLabel++);
}
