
CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDES =
LDFLAGS =
LDLIBS = -lpthread

PROGS =	sample_thread_nolock sample_thread_lock
LIBOBJS = interprocess_mutex.o

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


