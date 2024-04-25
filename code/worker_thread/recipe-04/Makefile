CXX = g++
CXXFLAGS = -g3 -Wall -Wextra #-DDEBUG
INCLUDE = 
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	sample_task_queue1 sample_task_queue2 \
		sample_worker_thread1 sample_worker_thread2 sample_worker_thread3

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

sample_task_queue1: sample_task_queue1.o worker_thread.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

sample_task_queue2: sample_task_queue2.o worker_thread.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

sample_worker_thread1: sample_worker_thread1.o worker_thread.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

sample_worker_thread2: sample_worker_thread2.o worker_thread.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

sample_worker_thread3: sample_worker_thread3.o worker_thread.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

