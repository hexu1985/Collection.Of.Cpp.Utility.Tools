CC = gcc
CFLAGS = -g3 -Wall -Wextra
INCLUDES = 
LDFLAGS = 
LDLIBS = 

PROGS =	fibonacci test1 watch

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

fibonacci: fibonacci.c
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

test1: test1.c
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

watch: watch.c
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

