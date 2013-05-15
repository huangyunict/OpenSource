PROGRAM     = test_lbfgsxx

SRC         = test_lbfgsxx.cpp
OBJ         = test_lbfgsxx.o 
ASM			= liblbfgs.a

CXX         = g++
CXXFLAGS    = -Wall -O2 -std=c++0x -I ../include -ffast-math

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

