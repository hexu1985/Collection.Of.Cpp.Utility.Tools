
CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDES = -Isrc
LDFLAGS =
LDLIBS = -lpthread -lrt

PROGS =	sample_call_once
LIBOBJS = interprocess_once.o
VPATH = src 

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $^ $(CXXFLAGS) $(INCLUDES)

sample_call_once: sample_call_once.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)




