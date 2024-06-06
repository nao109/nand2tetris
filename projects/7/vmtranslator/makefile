CXX  = g++
CXXFLAGS  = -std=gnu++17 -Wall -Wextra -fsanitize=undefined -O2 -DONLINE_JUDGE
INCDIR  = -I/opt/boost/gcc/include -I/opt/ac-library
LIBDIR  = -L/opt/boost/gcc/lib

FILE = StackTest.vm

main: VMtranslator.cpp $(FILE)
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) -o VMtranslator VMtranslator.cpp Parser.cpp CodeWriter.cpp
	./VMtranslator $(FILE)
