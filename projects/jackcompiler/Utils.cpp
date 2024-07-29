#include "Utils.hpp"

std::string kind2string(Kind kind){
    switch(kind){
        case Kind::STATIC:
            return "static";
            break;
        case Kind::FIELD:
            return "field";
        case Kind::ARG:
            return "argument";
            break;
        case Kind::VAR:
            return "var";
            break;
        default:
            return "none";
            break;
    }
}
