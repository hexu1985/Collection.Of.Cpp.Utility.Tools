CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -std=c++20
LDFLAGS = 
LDLIBS = -lpthread

SOURCES = $(shell ls *.cpp)
TARGETS = $(subst .cpp,,$(SOURCES))
#TARGETS = $(SOURCES:%.cpp=%)

all: $(TARGETS)
	@echo "TARGETS = $(TARGETS)" 

.PHONY:
clean:
	$(RM) $(TARGETS) a.out core *.o
	@echo "clean OK!"
