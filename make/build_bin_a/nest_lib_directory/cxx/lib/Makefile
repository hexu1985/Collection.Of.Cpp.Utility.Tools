CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
#AR = ar
#ARFLAGS = rv

OBJECTS = Message.o
TARGET = libmessage.a

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"
