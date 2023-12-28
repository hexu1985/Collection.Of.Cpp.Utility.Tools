CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDE = 
LDFLAGS = 
LDLIBS = 

PROGS =	random_random_class

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

random_random_class: random_random_class.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

