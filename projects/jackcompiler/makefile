CXX  = g++
CXXFLAGS  = -std=gnu++17 -Wall -Wextra -fsanitize=undefined -O2 -DONLINE_JUDGE -g
INCDIR  = -I/opt/boost/gcc/include -I/opt/ac-library
LIBDIR  = -L/opt/boost/gcc/lib

FILE = ArrayTest

./JackCompiler: main.cpp JackCompiler.cpp JackTokenizer.cpp CompilationEngine.cpp SymbolTable.cpp Utils.cpp JackCompiler.hpp JackTokenizer.hpp CompilationEngine.hpp SymbolTable.hpp Utils.hpp
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) -o JackCompiler main.cpp JackCompiler.cpp JackTokenizer.cpp CompilationEngine.cpp SymbolTable.cpp Utils.cpp

.PHONY: test
test: ./JackCompiler $(FILE)
	./JackCompiler $(FILE)
