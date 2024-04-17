CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -std=c++17
LDFLAGS = 
LDLIBS = -lgflags

PROGS =	streamer
LIBOBJS = SocketError.o Socket.o

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

streamer: streamer.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

