CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = 
LDLIBS = -lpthread

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

