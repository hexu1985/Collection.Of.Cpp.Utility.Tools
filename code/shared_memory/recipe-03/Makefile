
CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDES = -Isrc
LDFLAGS =
LDLIBS = -lgflags -lrt

PROGS =	shmcreate shmunlink shmread shmwrite shmem shmatomic
LIBOBJS = shared_memory_object.o
VPATH = src

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $^ $(CXXFLAGS) $(INCLUDES)

shmcreate: shmcreate.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

shmunlink: shmunlink.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

shmread: shmread.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

shmwrite: shmwrite.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

shmem: shmem.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

shmatomic: shmatomic.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

