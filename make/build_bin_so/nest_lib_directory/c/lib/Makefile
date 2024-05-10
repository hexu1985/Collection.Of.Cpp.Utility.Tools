CC = gcc
CFLAGS = -g3 -Wall -Wextra -fPIC
LDFLAGS = 

OBJECTS = hello.o
TARGET = libhello.so

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS)
	$(CC) -shared -o $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"
