CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -std=c++17
INCLUDES = 
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	no_pipeline manual_pipeline simple_pipeline_test \
		simple_composite_data_filter_test

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

simple_pipeline_test: simple_pipeline_test.cpp pipeline.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

simple_composite_data_filter_test: simple_composite_data_filter_test.cpp pipeline.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

