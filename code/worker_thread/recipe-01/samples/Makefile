
RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -DNDEBUG -fsanitize=leak -fno-omit-frame-pointer
INCLUDES = -I../include
LDFLAGS = -lpthread
LDPATH = 

SOURCES = $(shell ls *.cpp)
PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	@echo "OS: $(OS)"
	$(RM) $(PROGS)

%: %.cpp
	$(CXX) -o $@ ../src/worker_thread.cpp $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH) 
