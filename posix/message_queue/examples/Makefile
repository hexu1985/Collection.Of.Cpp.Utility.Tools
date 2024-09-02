
CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDES =
LDFLAGS =
LDLIBS = -lgflags -lrt

PROGS =	mqcreate1 mqcreate mqgetattr mqunlink mqsend mqreceive
LIBOBJS = posix_api.o

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $^ $(CXXFLAGS) $(INCLUDES)

mqcreate1: mqcreate1.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

mqcreate: mqcreate.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

mqgetattr: mqgetattr.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

mqunlink: mqunlink.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

mqsend: mqsend.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

mqreceive: mqreceive.o $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

