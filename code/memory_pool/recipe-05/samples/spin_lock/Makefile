
RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -DNDEBUG -std=c++11
INCLUDES = -I../../include -I../../../recipe-02/include -I../../../../spin_lock/recipe-01/include
LDFLAGS = 
LDPATH =
VPATH = 

SOURCES = $(shell ls *.cpp)
PROGS = sample_mt_memory_pool_spin_lock

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

$(PROGS): $(SOURCES)
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
