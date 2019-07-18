
RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -DNDEBUG -std=c++11
INCLUDES = -I../../../include
LDFLAGS = 
LDPATH =
VPATH = ../../../src
LIB_SOURCES = BulkMemoryPool.cpp

SOURCES = $(shell ls *.cpp)
PROGS = main

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

$(PROGS): $(SOURCES) $(LIB_SOURCES)
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
