
GFLAGS_DIR=$(HOME)/local/gflags

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra #-std=c++20
INCLUDES = -I$(GFLAGS_DIR)/include
LDFLAGS = -L$(GFLAGS_DIR)/lib
LDLIBS = -lgflags

PROGS =	streamer streamer6 blocks tcp_sixteen byteorder \
		local_machine_info udp_local
LIBOBJS = SocketException.o Socket.o SocketUtility.o

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $^ $(CXXFLAGS) $(INCLUDES)

streamer: streamer.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

streamer6: streamer6.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

blocks: blocks.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

tcp_sixteen: tcp_sixteen.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

byteorder: byteorder.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

local_machine_info: local_machine_info.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

udp_local: udp_local.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

