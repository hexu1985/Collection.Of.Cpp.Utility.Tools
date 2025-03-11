CXX = g++
CXXFLAGS = -Wall -std=c++17 -fsanitize=address -fno-omit-frame-pointer
LDFLAGS = 
LDLIBS = -lasan -lpthread

HEADERS = $(shell ls *.hpp)
SOURCES = $(shell ls *.cpp)
PROGRAMS = $(patsubst %.cpp,%,$(SOURCES))

.PHONY: all 
all: $(PROGRAMS)

$(PROGRAMS): %: %.o 
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o $(PROGRAMS)

