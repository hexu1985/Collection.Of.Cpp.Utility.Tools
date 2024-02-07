CXX = g++
CXXFLAGS = -g3 -Wall -Wextra #-DDEBUG
INCLUDE = 
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	sample timer_cancel timer_cancel2 timer_repeat timer_once \
		alarm_start alarm_start2

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

sample: sample.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

timer_cancel: timer_cancel.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

timer_cancel2: timer_cancel2.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

timer_repeat: timer_repeat.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

timer_once: timer_once.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_start: alarm_start.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_start2: alarm_start2.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)


