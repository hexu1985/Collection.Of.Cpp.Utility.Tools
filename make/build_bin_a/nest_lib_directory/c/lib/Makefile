CC = gcc
CFLAGS = -g3 -Wall -Wextra
#AR = ar
#ARFLAGS = rv

OBJECTS = hello.o
TARGET = libhello.a

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"
