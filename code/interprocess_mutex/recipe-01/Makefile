
CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDES =
LDFLAGS =
LDLIBS = -lpthread -lrt

PROGS =	sample_thread_nolock sample_thread_lock named_mutex_create named_mutex_remove print_pid_named_lock
LIBOBJS = interprocess_mutex.o shared_memory_object.o

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $^ $(CXXFLAGS) $(INCLUDES)

sample_thread_nolock: sample_thread_nolock.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

sample_thread_lock: sample_thread_lock.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

named_mutex_create: named_mutex_create.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

named_mutex_remove: named_mutex_remove.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

print_pid_named_lock: print_pid_named_lock.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)


