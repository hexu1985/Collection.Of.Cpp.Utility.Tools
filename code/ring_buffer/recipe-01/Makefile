
CXX = g++
CXXFLAGS = -g -Wall -Wextra
INCLUDES = -Isrc
LDFLAGS =
LDLIBS = -lpthread -lrt

PROGS =	sample_ringbuf
LIBOBJS = #interprocess_mutex.o
VPATH = src

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $^ $(CXXFLAGS) $(INCLUDES)

sample_ringbuf: sample_ringbuf.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

