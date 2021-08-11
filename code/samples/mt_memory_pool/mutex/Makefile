
RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -DNDEBUG -std=c++11
INCLUDES = -I../../../include
LDFLAGS = 
LDPATH =
VPATH = 

SOURCES = $(shell ls *.cpp)
PROGS = sample_mt_memory_pool_mutex

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

$(PROGS): $(SOURCES) $(LIB_SOURCES)
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
