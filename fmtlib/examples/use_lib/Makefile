FMTLIB_DIR = $(HOME)/local/fmtlib

CXX = g++
CXXFLAGS = -Wall -std=c++11 -I$(FMTLIB_DIR)/include
LDFLAGS = -Wl,-rpath,$(FMTLIB_DIR)/lib -Wl,--enable-new-dtags -L$(FMTLIB_DIR)/lib
LDLIBS = -lfmt

SOURCES = $(shell ls *.cpp)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))
PROGRAMS = $(patsubst %.cpp,%,$(SOURCES))

.PHONY: all 
all: $(PROGRAMS)

$(PROGRAMS): %: %.o
	$(CXX) $(LDFLAGS) -o $@ $< $(LDLIBS)

$(OBJECTS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(PROGRAMS)

