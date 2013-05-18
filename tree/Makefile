PROGRAM     = test

CXX         = g++
CXXFLAGS    = -Wall -O2
LINKFLAGS   = -Wall -O2

SRC         = test_tree.cc 
OBJ         = test_tree.o   

.SUFFIXES:
.SUFFIXES: .o .c .cc

.cc.o:
	$(CXX) $(CXXFLAGS) -c $*.cc

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CXX) $(LINKFLAGS) -o $(PROGRAM) $(OBJ)

clean:
	-rm -f *.o

cleanall: clean
	-rm $(PROGRAM)

