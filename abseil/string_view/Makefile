
ABSEIL_DIR=$(HOME)/local/abseil-cpp

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -std=c++17
INCLUDES = -I$(ABSEIL_DIR)/include
LDFLAGS = -labsl_string_view
LDPATH = -L$(ABSEIL_DIR)/lib

SOURCES = $(shell ls *.cpp)
PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

%: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
