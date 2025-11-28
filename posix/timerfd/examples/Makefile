CXX = g++
CXXFLAGS = -g -O0 -Wall -Wextra -I${HOME}/local/boost/include #-std=c++17
LDFLAGS = 
LDLIBS = -lpthread -lrt #-lboost_thread -lboost_system -lpthread -lrt

PROGRAM = test_timer_thread2

SOURCES = $(shell ls *.cpp)
HEADERS = $(shell ls *.h)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

.PHONY: all 
all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(PROGRAM) $(OBJECTS) $(LDLIBS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(PROGRAM)


