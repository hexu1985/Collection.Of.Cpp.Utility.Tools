CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDE = 
LDFLAGS = 
LDLIBS = 

PROGS =	TestRandom Fig10_63

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

TestRandom: TestRandom.o Random.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

Fig10_63: Fig10_63.o Random.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

