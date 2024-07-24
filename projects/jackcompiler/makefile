CXX  = g++
CXXFLAGS  = -std=gnu++17 -Wall -Wextra -fsanitize=undefined -O2 -DONLINE_JUDGE -g
INCDIR  = -I/opt/boost/gcc/include -I/opt/ac-library
LIBDIR  = -L/opt/boost/gcc/lib

FILE = ArrayTest

./JackAnalyzer: main.cpp JackAnalyzer.cpp JackTokenizer.cpp CompilationEngine.cpp JackAnalyzer.hpp JackTokenizer.hpp CompilationEngine.hpp Utils.hpp 
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) -o JackAnalyzer main.cpp JackAnalyzer.cpp JackTokenizer.cpp CompilationEngine.cpp

.PHONY: test
test: ./JackAnalyzer $(FILE)
	./JackAnalyzer $(FILE)
