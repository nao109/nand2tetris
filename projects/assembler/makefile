CXX  = g++
CXXFLAGS  = -std=gnu++17 -Wall -Wextra -fsanitize=undefined -O2 -DONLINE_JUDGE
INCDIR  = -I/opt/boost/gcc/include
LIBDIR  = -L/opt/boost/gcc/lib

FILE = Add.asm

./Assembler: main.cpp Code.cpp Parser.cpp SymbolTable.cpp Assembler.hpp
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) -o Assembler main.cpp Code.cpp Parser.cpp SymbolTable.cpp

.PHONY: test
test: ./Assembler $(FILE)
	./Assembler $(FILE)
