CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	MyThreadClass

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

MyThreadClass: MyThreadClass.o Thread.o Random.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

