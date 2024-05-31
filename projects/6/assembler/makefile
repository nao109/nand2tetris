CXX  = g++
CXXFLAGS  = -std=gnu++17 -Wall -Wextra -fsanitize=undefined -O2 -DONLINE_JUDGE
INCDIR  = -I/opt/boost/gcc/include
LIBDIR  = -L/opt/boost/gcc/lib

FILE = Add.asm

main: Assembler.cpp Code.cpp Parser.cpp SymbolTable.cpp Assembler.hpp $(FILE)
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) -o Assembler Assembler.cpp Code.cpp Parser.cpp SymbolTable.cpp
	./Assembler $(FILE)
