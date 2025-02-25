CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	sample_frame_delay_checker1 sample_frame_delay_checker2

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

sample_frame_delay_checker1: sample_frame_delay_checker1.o frame_delay_checker.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

sample_frame_delay_checker2: sample_frame_delay_checker2.o frame_delay_checker.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

