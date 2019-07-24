# Makefile

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++11
INCLUDE = -I../../include
LDFLAGS = 
LDPATH = 

PROGS =	sample_bulk_memory_pool1

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

sample_mt_memory_pool1: sample_mt_memory_pool1.o ../dummylock/rational.o BulkMemoryPool.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)


