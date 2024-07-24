#include "JackAnalyzer.hpp"

int main(int argc, char *argv[]) {
    if(argc != 2){
        cerr << "Cannot start program\n";
        return 1;
    }

    fs::path inputFile = argv[1];

    if(!fs::exists(inputFile)){
        cerr << "File does not exist\n";
        return 1;
    }

    JackAnalyzer analyzer(inputFile);
    analyzer.analyze();

    return 0;
}
