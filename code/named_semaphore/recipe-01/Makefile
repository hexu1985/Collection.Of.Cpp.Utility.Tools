CXX = g++
CXXFLAGS = -g -Wall -Wextra
INCLUDES = -Isrc -Iinterprocess_mutex -Iinterprocess_condition -Iinterprocess_semaphore -Ishared_memory -Iinterprocess_once
LDFLAGS =
LDLIBS = -lgflags -lpthread -lrt

PROGS =	semcreate semunlink sempost semwait semtrywait semgetvalue prodcons1
LIBOBJS = named_semaphore.o interprocess_mutex.o interprocess_condition.o interprocess_semaphore.o shared_memory_object.o interprocess_once.o
VPATH = src interprocess_mutex interprocess_condition interprocess_semaphore shared_memory interprocess_once

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $^ $(CXXFLAGS) $(INCLUDES)

semcreate: semcreate.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

semunlink: semunlink.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

sempost: sempost.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

semwait: semwait.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

semtrywait: semtrywait.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

semgetvalue: semgetvalue.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

prodcons1: prodcons1.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)



