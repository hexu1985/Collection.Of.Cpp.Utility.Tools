CXX = g++
CXXFLAGS = -Wall -std=c++17

SOURCES = $(shell ls *.cpp)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))
PROGRAMS = $(patsubst %.cpp,%,$(SOURCES))

.PHONY: all 
all: $(PROGRAMS)

$(PROGRAMS): %: %.o
	$(CXX) -o $@ $<

$(OBJECTS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(PROGRAMS)

