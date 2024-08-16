
GBENCH_DIR=$(HOME)/local/google_benchmark

RM = rm -f
CXX = clang++
CXXFLAGS = -g -O3 -mavx2 -Wall -pedantic
INCLUDES = -I$(GBENCH_DIR)/include -I.. -I../src -I../shared_memory
LDLIBS = -pthread -lbenchmark
LDFLAGS = -Wl,-rpath,$(GBENCH_DIR)/lib -Wl,--enable-new-dtags -L$(GBENCH_DIR)/lib
VPATH = ../src ../shared_memory

PROGS = std_mutex_incr named_mutex_incr

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

std_mutex_incr: std_mutex_incr.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

named_mutex_incr: named_mutex_incr.cpp named_mutex.cpp shared_memory_object.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS) -lrt

