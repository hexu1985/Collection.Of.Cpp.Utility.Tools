GPERFTOOLS_DIR = $(HOME)/local/fmtlib

CXX = g++
CXXFLAGS = -Wall -std=c++11 -I$(GPERFTOOLS_DIR)/include
LDFLAGS = -Wl,-rpath,$(GPERFTOOLS_DIR)/lib -Wl,--enable-new-dtags -L$(GPERFTOOLS_DIR)/lib
LDLIBS = -ltcmalloc

SOURCES = $(shell ls *.cpp)
PROGRAMS = $(patsubst %.cpp,%,$(SOURCES))

.PHONY: all 
all: $(PROGRAMS)

$(PROGRAMS): %: %.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o $(PROGRAMS)

