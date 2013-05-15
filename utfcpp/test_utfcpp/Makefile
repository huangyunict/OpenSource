PROGRAM     = test_utfcpp

SRC         = test_utfcpp.cpp
OBJ         = test_utfcpp.o 
ASM			= 

CXX         = g++
CXXFLAGS    = -Wall -O2 -std=c++0x -I ../include

LINKFLAGS   = -Wall -O2 

.SUFFIXES:
.SUFFIXES: .o .c .cpp
.PHONY: all clean cleanall

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $*.cpp

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CXX) $(LINKFLAGS) -o $(PROGRAM) $(OBJ) $(ASM)

clean:
	-rm -f *.o

cleanall: clean
	-rm $(PROGRAM)

