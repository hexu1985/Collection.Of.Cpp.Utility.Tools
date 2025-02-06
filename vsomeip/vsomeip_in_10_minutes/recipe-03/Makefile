
VSOMEIP_DIR=$(HOME)/local/vsomeip

RM = rm -f
CXX = clang++
CXXFLAGS = -g -O3 -mavx2 -Wall -pedantic
INCLUDES = -I$(VSOMEIP_DIR)/include
LDLIBS = -lvsomeip3 -lpthread 
LDFLAGS = -Wl,-rpath,$(VSOMEIP_DIR)/lib -Wl,--enable-new-dtags -L$(VSOMEIP_DIR)/lib
VPATH = 

PROGS = service-example client-example

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

service-example: service-example.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

client-example: client-example.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

