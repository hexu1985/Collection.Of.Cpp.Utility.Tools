CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -I/usr/include/eigen3
CC = g++	# for link
LDFLAGS =
LDLIBS =

SOURCES = $(shell ls *.cpp)
TARGETS = $(subst .cpp,,$(SOURCES))
#TARGETS = $(SOURCES:%.cpp=%)

all: $(TARGETS)
	@echo "TARGETS = $(TARGETS)" 

$(TARGETS): %: %.o

.PHONY:
clean:
	$(RM) $(TARGETS) a.out core *.o
	@echo "clean OK!"
