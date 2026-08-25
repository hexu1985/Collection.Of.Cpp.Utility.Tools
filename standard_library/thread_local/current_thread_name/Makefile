CXX = g++
CXXFLAGS = -Wall -std=c++11
PROGRAM = current_thread_name

SOURCES = $(shell ls *.cpp)
HEADERS = $(shell ls *.hpp)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

.PHONY: all 
all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(PROGRAM)

