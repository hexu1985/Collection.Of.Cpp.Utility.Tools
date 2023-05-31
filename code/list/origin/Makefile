CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
CC = g++	# for link

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
