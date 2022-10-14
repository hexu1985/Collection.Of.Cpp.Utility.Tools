
RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -DNDEBUG -std=c++11
INCLUDES = 
LDFLAGS = -lpthread
LDPATH =
VPATH = 

SOURCES = $(shell ls *.cpp)
#OBJECTS = $(SOURCES:%.cpp=%.o)
OBJECTS = $(subst .cpp,.o,$(SOURCES))
PROGS = test

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS) $(OBJECTS)

$(OBJECTS): %.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(INCLUDES) $<

$(PROGS): $(OBJECTS)
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
