
VSOMEIP_DIR=$(HOME)/local/vsomeip

RM = rm -f
CXX = clang++
CXXFLAGS = -g -O3 -mavx2 -Wall -pedantic
INCLUDES = -I$(VSOMEIP_DIR)/include
LDLIBS = -lvsomeip3 -lpthread 
LDFLAGS = -Wl,-rpath,$(VSOMEIP_DIR)/lib -Wl,--enable-new-dtags -L$(VSOMEIP_DIR)/lib
VPATH = 

PROGS = request-sample response-sample

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

response-sample: response-sample.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

request-sample: request-sample.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

