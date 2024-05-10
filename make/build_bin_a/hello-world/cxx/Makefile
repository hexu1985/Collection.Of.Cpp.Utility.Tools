CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
#AR = ar
#ARFLAGS = rv

OBJECTS = hello-world.o
TARGET = hello-world

LIB_OBJS = Message.o
LIB_NAME = libmessage.a

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS) $(LIB_NAME)
	$(CXX) -o $@ $^

$(LIB_NAME): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) $(LIB_NAME) a.out core *.o
	@echo "clean OK!"
