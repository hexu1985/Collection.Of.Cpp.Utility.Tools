
GBENCH_DIR=$(HOME)/local/google_benchmark

RM = rm -f
CXX = clang++
CXXFLAGS = -g -O3 -mavx2 -Wall -pedantic
INCLUDES = -I$(GBENCH_DIR)/include -I..
LDLIBS = -pthread -lbenchmark
LDFLAGS = -Wl,-rpath,$(GBENCH_DIR)/lib -Wl,--enable-new-dtags -L$(GBENCH_DIR)/lib
VPATH = ..

PROGS = std_mutex_incr spin_lock_incr

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

std_mutex_incr: std_mutex_incr.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

spin_lock_incr: spin_lock_incr.cpp 
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

