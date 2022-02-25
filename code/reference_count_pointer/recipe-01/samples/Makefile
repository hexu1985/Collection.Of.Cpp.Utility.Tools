
OS ?= $(shell uname -s)

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -DNDEBUG
INCLUDES = -I../include
LDFLAGS = -lpthread
LDPATH = #-L../src
VPATH = ../src

PROGS =	benchmark_rc_ptr sample_rc_ptr

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	@echo "OS: $(OS)"
	$(RM) $(PROGS) *.o

%.o: %.cpp
	$(CXX) -c $(INCLUDES) $(CXXFLAGS) -o $@ $^

benchmark_rc_ptr:	widget.o benchmark_rc_ptr.o rc_object.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDPATH) $(LDFLAGS)

sample_rc_ptr:	widget.o sample_rc_ptr.o rc_object.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDPATH) $(LDFLAGS)

