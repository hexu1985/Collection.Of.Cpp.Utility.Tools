
OS ?= $(shell uname -s)

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -DNDEBUG
INCLUDES = -I../../include
LDFLAGS = -lmini_utils -lpthread
LDPATH = -L../../src
#VPATH = ../../src

PROGS =	benchmark_rci_ptr sample_rci_ptr

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	@echo "OS: $(OS)"
	$(RM) $(PROGS) *.o

%.o: %.cpp
	$(CXX) -c $(INCLUDES) $(CXXFLAGS) -o $@ $^

benchmark_rci_ptr:	widget.o benchmark_rci_ptr.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDPATH) $(LDFLAGS)

sample_rci_ptr:	widget.o sample_rci_ptr.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDPATH) $(LDFLAGS)

