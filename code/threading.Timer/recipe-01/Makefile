CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -DDEBUG
INCLUDE = 
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	alarm_start alarm_cancel alarm_cancel2 alarm_repeat

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

alarm_start: alarm_start.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_cancel: alarm_cancel.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_cancel2: alarm_cancel2.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_repeat: alarm_repeat.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

