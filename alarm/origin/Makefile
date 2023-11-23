CC = gcc
CFLAGS = -g3 -Wall -Wextra -DDEBUG
LDLIBS = -lpthread

SOURCES = $(shell ls *.c)
TARGETS = $(subst .c,,$(SOURCES))
#TARGETS = $(SOURCES:%.c=%)

all: $(TARGETS)
	@echo "TARGETS = $(TARGETS)" 

.PHONY:
clean:
	$(RM) $(TARGETS) a.out core *.o
	@echo "clean OK!"

