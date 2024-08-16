
CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDES = -Isrc -Iinterprocess_mutex
LDFLAGS =
LDLIBS = -lpthread -lrt

PROGS =	sample_wait sample_notify_one sample_notify_all
LIBOBJS = interprocess_mutex.o interprocess_condition.o
VPATH = src interprocess_mutex

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $^ $(CXXFLAGS) $(INCLUDES)

sample_wait: sample_wait.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

sample_notify_one: sample_notify_one.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

sample_notify_all: sample_notify_all.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

