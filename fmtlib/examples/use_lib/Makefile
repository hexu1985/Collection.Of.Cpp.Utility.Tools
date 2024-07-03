
FMTLIB_ROOT = $(HOME)/local/fmtlib
RM = rm -f
CXX = g++
CXXFLAGS = -Wall 
INCLUDES = -I../include -I$(FMTLIB_ROOT)/include
LDFLAGS = -lfmt
LDPATH = -L$(FMTLIB_ROOT)/lib

SOURCES = $(shell ls *.cpp)
PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

%: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
