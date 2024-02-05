CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDES = 
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	no_pipeline manual_pipeline

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

no_pipeline: no_pipeline.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

manual_pipeline: manual_pipeline.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

