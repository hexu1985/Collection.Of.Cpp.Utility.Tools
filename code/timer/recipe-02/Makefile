CXX = g++
CXXFLAGS = -g3 -Wall -Wextra #-DDEBUG
INCLUDE = 
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	alarm_start timer_cancel timer_cancel2 timer_repeat timer_once

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

alarm_start: alarm_start.o timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_start2: alarm_start2.o timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

timer_cancel: timer_cancel.o timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

timer_cancel2: timer_cancel2.o timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

timer_repeat: timer_repeat.o timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

timer_once: timer_once.o timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

