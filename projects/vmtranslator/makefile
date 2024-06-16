CXX  = g++
CXXFLAGS  = -std=gnu++17 -Wall -Wextra -fsanitize=undefined -O2 -DONLINE_JUDGE
INCDIR  = -I/opt/boost/gcc/include -I/opt/ac-library
LIBDIR  = -L/opt/boost/gcc/lib

FILE = StaticTest.vm

main: main.cpp VMtranslator.cpp Parser.cpp CodeWriter.cpp
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) -o VMtranslator main.cpp VMtranslator.cpp Parser.cpp CodeWriter.cpp

test: ./VMtranslator $(FILE)
	./VMtranslator $(FILE)
