# Makefile

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -fsanitize=address -fno-omit-frame-pointer
INCLUDE = 
LDFLAGS = -lasan
LDPATH = 
VPATH = ..

PROGS =	sample_memory_pool1

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

sample_memory_pool1: sample_memory_pool1.o rational.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)

