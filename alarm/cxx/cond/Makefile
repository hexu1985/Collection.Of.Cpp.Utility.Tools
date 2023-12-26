CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -DDEBUG
INCLUDE = 
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	alarm_cond alarm_cond2 cancel_alarm_cond cancel_alarm_cond2

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

alarm_cond: alarm_cond.o timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_cond2: alarm_cond2.o timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

cancel_alarm_cond: cancel_alarm_cond.o timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

cancel_alarm_cond2: cancel_alarm_cond2.o timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

