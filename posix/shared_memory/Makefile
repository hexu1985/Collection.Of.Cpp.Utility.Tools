
CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDES =
LDFLAGS =
LDLIBS = -lgflags -lrt

PROGS =	shmcreate shmunlink shmread shmwrite
LIBOBJS = posix_api.o

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

