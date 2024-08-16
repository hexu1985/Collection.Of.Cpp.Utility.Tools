CXX = g++
CXXFLAGS = -g3 -Wall -Wextra #-std=c++17
LDFLAGS = 
LDLIBS = -lboost_thread -lboost_system -lpthread -lgflags

SOURCES = $(shell ls *.cpp)
TARGETS = $(subst .cpp,,$(SOURCES))
#TARGETS = $(SOURCES:%.cpp=%)

all: $(TARGETS)
	@echo "TARGETS = $(TARGETS)" 

.PHONY:
clean:
	$(RM) $(TARGETS) a.out core *.o
	@echo "clean OK!"
