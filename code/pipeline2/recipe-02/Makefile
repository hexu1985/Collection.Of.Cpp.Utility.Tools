CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDES = 
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	no_pipeline manual_pipeline simple_pipeline_test pipeline_test \
		simple_pipeline_sink_test pipeline_add_source_test

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

pipeline_test: pipeline_test.cpp process_node.cpp data_filter_any.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

simple_pipeline_test: simple_pipeline_test.cpp process_node.cpp data_filter_any.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

simple_pipeline_sink_test: simple_pipeline_sink_test.cpp process_node.cpp data_filter_any.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

pipeline_add_source_test: pipeline_add_source_test.cpp process_node.cpp data_filter_any.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

