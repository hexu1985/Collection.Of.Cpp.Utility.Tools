
VSOMEIP_DIR=$(HOME)/local/vsomeip

RM = rm -f
CXX = clang++
CXXFLAGS = -g -O3 -mavx2 -Wall -pedantic
INCLUDES = -I$(VSOMEIP_DIR)/include
LDLIBS = -lvsomeip3 -lpthread 
LDFLAGS = -Wl,-rpath,$(VSOMEIP_DIR)/lib -Wl,--enable-new-dtags -L$(VSOMEIP_DIR)/lib
VPATH = 

PROGS = hello_world_service hello_world_client

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

hello_world_service: hello_world_service_main.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

hello_world_client: hello_world_client_main.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

