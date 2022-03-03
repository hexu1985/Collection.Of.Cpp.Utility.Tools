# Makefile

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -fsanitize=address -fno-omit-frame-pointer
INCLUDE = -I../../include
LDFLAGS = -lasan
LDPATH = 
VPATH = .. ../../src

PROGS =	sample_byte_memory_pool1

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

sample_byte_memory_pool1: sample_byte_memory_pool1.o rational.o byte_memory_pool.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)


