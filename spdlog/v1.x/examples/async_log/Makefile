
SPDLOG_ROOT = $(HOME)/local/spdlog
RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g #-DSPDLOG_DEBUG_ON -DSPDLOG_TRACE_ON 
INCLUDES = -I$(SPDLOG_ROOT)/include
LDFLAGS = -lspdlog -lpthread
LDPATH = -L$(SPDLOG_ROOT)/lib

SOURCES = $(shell ls *.cpp)
PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS) logs/* myfilename logfile_*

%: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
