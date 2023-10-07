CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDE = 
LDFLAGS = 
LDLIBS = 

PROGS =	random_random_class random_randint random_seed 

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

random_random_class: random_random_class.o Random.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

random_randint: random_randint.o Random.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

random_seed: random_seed.o Random.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

