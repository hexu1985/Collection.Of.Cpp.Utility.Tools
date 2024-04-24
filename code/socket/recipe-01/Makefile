CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -std=c++17
LDFLAGS = 
LDLIBS = -lgflags

PROGS =	streamer streamer6 blocks
LIBOBJS = SocketException.o Socket.o

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

streamer: streamer.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

streamer6: streamer6.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

blocks: blocks.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

