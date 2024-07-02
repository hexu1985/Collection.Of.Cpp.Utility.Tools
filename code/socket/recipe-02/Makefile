
ABSEIL_DIR=$(HOME)/local/abseil

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDES = -I$(ABSEIL_DIR)/include
LDFLAGS = -L$(ABSEIL_DIR)/lib
LDLIBS = -lgflags -labsl_string_view

PROGS =	streamer streamer6 blocks tcp_sixteen byteorder udp_local
LIBOBJS = SocketException.o Socket.o SocketUtility.o SocketAddress.o

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

udp_local: udp_local.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

