
RM = rm -f
CXX = g++
CXXFLAGS = -Wall 
INCLUDES = -IBigNumber/src -IBigNumber
LDFLAGS = -lpthread
LDPATH =

SOURCES = $(shell ls *.cpp)
PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

%: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ BigNumber/src/bignumber.cpp $(LDFLAGS) $(LDPATH)
