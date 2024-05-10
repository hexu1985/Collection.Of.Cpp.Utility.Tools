CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDES = 
LDFLAGS = 
LDLIBS = 

PROGS =	queue_back_test queue_empty_test queue_front_test queue_pop_test

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

queue_back_test: queue_back_test.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

queue_empty_test: queue_empty_test.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

queue_front_test: queue_front_test.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

queue_pop_test: queue_pop_test.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

