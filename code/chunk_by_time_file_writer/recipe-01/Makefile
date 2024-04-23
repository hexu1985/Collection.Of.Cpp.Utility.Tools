CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDE = 
LDFLAGS = 
LDLIBS = 

PROGS =	test_chunk_by_time_file_writer

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

test_chunk_by_time_file_writer: test_chunk_by_time_file_writer.o chunk_by_time_file_writer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

