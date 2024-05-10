CC = gcc
CFLAGS = -g3 -Wall -Wextra

INCLUDES = -Ilib
LDFLAGS = -Llib
LDLIBS = -lhello

OBJECTS = main.o
TARGET = hello-world

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS)
	$(MAKE) --directory=lib
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)

.PHONY:
clean:
	$(MAKE) clean --directory=lib
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"
