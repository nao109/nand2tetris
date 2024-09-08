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
