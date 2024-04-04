#ifndef CODE_H_
#define CODE_H_

#include "Assembler.hpp"

namespace Assembler {
    namespace Code {
        string dest(string dst){
            // cerr << "dest: " << dst << "\n";
            if(dst == "") return "000";
            if(dst == "M") return "001";
            if(dst == "D") return "010";
            if(dst == "MD") return "011";
            if(dst == "A") return "100";
            if(dst == "AM") return "101";
            if(dst == "AD") return "110";
            if(dst == "AMD") return "111";
            return "";
        }

        string comp(string cmp){
            // cerr << "cmp: " << cmp << "\n";
            if(cmp == "0")   return "0101010";
            if(cmp == "1")   return "0111111";
            if(cmp == "-1")  return "0111010";
            if(cmp == "D")   return "0001100";
            if(cmp == "A")   return "0110000";
            if(cmp == "!D")  return "0001101";
            if(cmp == "!A")  return "0110001";
            if(cmp == "-D")  return "0001111";
            if(cmp == "-A")  return "0110011";
            if(cmp == "D+1") return "0011111";
            if(cmp == "A+1") return "0110111";
            if(cmp == "D-1") return "0001110";
            if(cmp == "A-1") return "0110010";
            if(cmp == "D+A") return "0000010";
            if(cmp == "D-A") return "0010011";
            if(cmp == "A-D") return "0000111";
            if(cmp == "D&A") return "0000000";
            if(cmp == "D|A") return "0010101";
            if(cmp == "M")   return "1110000";
            if(cmp == "!M")  return "1110001";
            if(cmp == "-M")  return "1110011";
            if(cmp == "M+1") return "1110111";
            if(cmp == "M-1") return "1110010";
            if(cmp == "D+M") return "1000010";
            if(cmp == "D-M") return "1010011";
            if(cmp == "M-D") return "1000111";
            if(cmp == "D&M") return "1000000";
            if(cmp == "D|M") return "1010101";
            return "";
        }

        string jump(string jmp){
            // cerr << "jmp: " << jmp << "\n";
            if(jmp == "") return "000";
            if(jmp == "JGT") return "001";
            if(jmp == "JEQ") return "010";
            if(jmp == "JGE") return "011";
            if(jmp == "JLT") return "100";
            if(jmp == "JNE") return "101";
            if(jmp == "JLE") return "110";
            if(jmp == "JMP") return "111";
            return "";
        }
    }
}

#endif  // CODE_H_
