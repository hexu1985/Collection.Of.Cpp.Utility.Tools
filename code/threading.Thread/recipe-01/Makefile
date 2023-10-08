CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	MyThreadClass MyThreadClass_lock

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

MyThreadClass: MyThreadClass.o Thread.o Random.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

MyThreadClass_lock: MyThreadClass_lock.o Thread.o Random.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

