
CXX = g++
CXXFLAGS = -g -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer
INCLUDES = -Isrc
LDFLAGS =
LDLIBS = -lasan

PROGS =	sample_ringbuf sample_prodcons1
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

sample_prodcons1: sample_prodcons1.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

