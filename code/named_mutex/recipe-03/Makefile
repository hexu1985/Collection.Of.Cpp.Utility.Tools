
CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDES = -Isrc -Ishared_memory -Iinterprocess_mutex -Iinterprocess_once
LDFLAGS =
LDLIBS = -lgflags -lpthread -lrt

PROGS =	named_mutex_create named_mutex_remove print_pid_named_lock
LIBOBJS = shared_memory_object.o named_mutex.o interprocess_once.o interprocess_mutex.o
VPATH = src shared_memory interprocess_mutex interprocess_once

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $^ $(CXXFLAGS) $(INCLUDES)

named_mutex_create: named_mutex_create.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

named_mutex_remove: named_mutex_remove.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

print_pid_named_lock: print_pid_named_lock.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)



