
CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDES =
LDFLAGS =
LDLIBS = -lgflags -lpthread -lrt

PROGS =	semcreate semunlink semwait semtrywait sempost semgetvalue prodcons1 prodcons2 prodcons3 prodcons4 \
		mycat1 mycat2 deadlock
LIBOBJS = posix_api.o

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

semwait: semwait.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

semtrywait: semtrywait.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

sempost: sempost.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

semgetvalue: semgetvalue.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

prodcons1: prodcons1.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

prodcons2: prodcons2.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

prodcons3: prodcons3.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

prodcons4: prodcons4.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

mycat1: mycat1.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

mycat2: mycat2.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

deadlock: deadlock.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)


