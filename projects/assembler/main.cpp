#include "Assembler.hpp"

int main(int argc, char *argv[]) {
    if(argc != 2){
        cerr << "Cannot start program\n";
        return 1;
    }

    string input, output;
    input = output = argv[1];
    output.replace(output.rfind("asm"), 3, "hack");

    ifstream ifs(input);
    if(ifs.fail()){
        cerr << "Cannot open file\n";
        return 1;
    }

    Assembler assembler;
    assembler.assemble(input, output);

    return 0;
}
