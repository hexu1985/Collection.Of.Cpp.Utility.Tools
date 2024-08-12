CXX = g++
CXXFLAGS = -g3 -Wall -Wextra #-DDEBUG
INCLUDE = 
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	print_pid_nolock

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

print_pid_nolock: print_pid_nolock.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

