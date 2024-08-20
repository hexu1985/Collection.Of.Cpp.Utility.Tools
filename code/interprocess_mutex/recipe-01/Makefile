
CXX = g++
CXXFLAGS = -g -Wall -Wextra
INCLUDES = -Isrc
LDFLAGS =
LDLIBS = -lpthread -lrt

PROGS =	sample_lock sample_mutex sample_native_handle sample_try_lock
LIBOBJS = interprocess_mutex.o
VPATH = src

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $^ $(CXXFLAGS) $(INCLUDES)

sample_lock: sample_lock.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

sample_mutex: sample_mutex.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

sample_native_handle: sample_native_handle.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

sample_try_lock: sample_try_lock.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)
