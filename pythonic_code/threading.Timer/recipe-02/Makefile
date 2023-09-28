CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -DDEBUG
INCLUDE = 
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	alarm_start alarm_cancel alarm_cancel2

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

alarm_start: alarm_start.o Timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_cancel: alarm_cancel.o Timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_cancel2: alarm_cancel2.o Timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

