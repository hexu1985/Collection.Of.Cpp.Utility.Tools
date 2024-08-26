CXX = g++
CXXFLAGS = -g -Wall -Wextra
INCLUDES = -Isrc -Iinterprocess_mutex -Iinterprocess_condition
LDFLAGS =
LDLIBS = -lpthread -lrt

PROGS =	prodcons2 prodcons3 prodcons4 mycat1 mycat2
LIBOBJS = semaphore.o
VPATH = src 

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $^ $(CXXFLAGS) $(INCLUDES)

prodcons2: prodcons2.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

prodcons3: prodcons3.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

prodcons4: prodcons4.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

mycat1: mycat1.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

mycat2: mycat2.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)



