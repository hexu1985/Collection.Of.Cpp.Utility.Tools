CC = gcc
CFLAGS = -g3 -Wall -Wextra
#AR = ar
#ARFLAGS = rv

OBJECTS = main.o
TARGET = hello-world

LIB_OBJS = hello.o
LIB_NAME = libhello.a

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS) $(LIB_NAME)
	$(CC) -o $@ $^

$(LIB_NAME): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) $(LIB_NAME) a.out core *.o
	@echo "clean OK!"
