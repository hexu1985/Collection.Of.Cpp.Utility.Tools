
CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDES =
LDFLAGS =
LDLIBS = -lgflags -lrt

PROGS =	shm_create shm_remove shm_read shm_write
LIBOBJS = posix_api.o

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $^ $(CXXFLAGS) $(INCLUDES)

shm_create: shm_create.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

shm_remove: shm_remove.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

shm_read: shm_read.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

shm_write: shm_write.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

