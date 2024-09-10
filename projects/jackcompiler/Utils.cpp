#include "Utils.hpp"

std::string kind2string(Kind kind){
    switch(kind){
        case Kind::STATIC:
            return "static";
        case Kind::FIELD:
            return "field";
        case Kind::ARG:
            return "argument";
        case Kind::VAR:
            return "var";
        default:
            return "none";
    }
}

Command op2command(std::string op){
    if(op == "+") return Command::ADD;
    else if(op == "-") return Command::SUB;
    else if(op == "&") return Command::AND;
    else if(op == "|") return Command::OR;
    else if(op == "<") return Command::LT;
    else if(op == ">") return Command::GT;
    else if(op == "=") return Command::EQ;
    else return Command::NONE;
}

Command unaryop2command(std::string unaryOp){
    if(unaryOp == "-") return Command::NEG;
    else if(unaryOp == "~") return Command::NOT;
    else return Command::NONE;
}
