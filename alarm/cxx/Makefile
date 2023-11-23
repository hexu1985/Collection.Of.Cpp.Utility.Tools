CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -DDEBUG
INCLUDE = 
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	alarm_thread alarm_thread2 alarm_cond alarm_cond2 \
		cancel_alarm_thread cancel_alarm_thread2 \
		cancel_alarm_cond cancel_alarm_cond2

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

alarm_thread: alarm_thread.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_thread2: alarm_thread2.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_cond: alarm_cond.o cond_timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_cond2: alarm_cond2.o cond_timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

cancel_alarm_thread: cancel_alarm_thread.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

cancel_alarm_thread2: cancel_alarm_thread2.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

cancel_alarm_cond: cancel_alarm_cond.o cond_timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

cancel_alarm_cond2: cancel_alarm_cond2.o cond_timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

