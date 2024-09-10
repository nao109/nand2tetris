#include "VMWriter.hpp"

VMWriter::VMWriter(std::filesystem::path inputFile) {
    // 出力jackファイル
    std::string outputFile = inputFile.string();
    outputFile.replace(outputFile.rfind(".jack"), 5, ".vm");

    ofs.open(outputFile);
    if(ofs.fail()){
        std::cerr << "Cannot open file: " << outputFile << "\n";
    }
}

void VMWriter::writePush(Segment segment, int index){
    switch(segment){
        case Segment::CONST:
            ofs << "push constant " << index << "\n";
            break;
        case Segment::ARG:
            ofs << "push argument " << index << "\n";
            break;
        case Segment::LOCAL:
            ofs << "push local " << index << "\n";
            break;
        case Segment::STATIC:
            ofs << "push static " << index << "\n";
            break;
        case Segment::THIS:
            ofs << "push this " << index << "\n";
            break;
        case Segment::THAT:
            ofs << "push that " << index << "\n";
            break;
        case Segment::POINTER:
            ofs << "push pointer " << index << "\n";
            break;
        case Segment::TEMP:
            ofs << "push temp " << index << "\n";
            break;
        default:
            break;
    }
}

void VMWriter::writePop(Segment segment, int index){
    switch(segment){
        case Segment::CONST:
            ofs << "pop constant " << index << "\n";
            break;
        case Segment::ARG:
            ofs << "pop argument " << index << "\n";
            break;
        case Segment::LOCAL:
            ofs << "pop local " << index << "\n";
            break;
        case Segment::STATIC:
            ofs << "pop static " << index << "\n";
            break;
        case Segment::THIS:
            ofs << "pop this " << index << "\n";
            break;
        case Segment::THAT:
            ofs << "pop that " << index << "\n";
            break;
        case Segment::POINTER:
            ofs << "pop pointer " << index << "\n";
            break;
        case Segment::TEMP:
            ofs << "pop temp " << index << "\n";
            break;
        default:
            break;
    }
}

void VMWriter::writeArithmetic(Command command){
    switch(command){
        case Command::ADD:
            ofs << "add\n";
            break;
        case Command::SUB:
            ofs << "sub\n";
            break;
        case Command::NEG:
            ofs << "neg\n";
            break;
        case Command::EQ:
            ofs << "eq\n";
            break;
        case Command::GT:
            ofs << "gt\n";
            break;
        case Command::LT:
            ofs << "lt\n";
            break;
        case Command::AND:
            ofs << "and\n";
            break;
        case Command::OR:
            ofs << "or\n";
            break;
        case Command::NOT:
            ofs << "not\n";
            break;
        default:
            break;
    }
}

void VMWriter::writeLabel(std::string label){
    ofs << "label " << label << "\n";
}

void VMWriter::writeGoto(std::string label){
    ofs << "goto " << label << "\n";
}

void VMWriter::writeIf(std::string label){
    ofs << "if-goto " << label << "\n";
}

void VMWriter::writeCall(std::string name, int nArgs){
    ofs << "call " << name << " " << nArgs << "\n";
}

void VMWriter::writeFunction(std::string name, int nLocals){
    ofs << "function " << name << " " << nLocals << "\n";
}

void VMWriter::writeReturn(){
    ofs << "return\n";
}

void VMWriter::close(){
    ofs.close();
}
