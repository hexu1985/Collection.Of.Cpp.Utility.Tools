CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -fPIC
LDFLAGS = -Wl,-rpath,'$$ORIGIN' -Wl,--enable-new-dtags

OBJECTS = hello-world.o
TARGET = hello-world

LIB_OBJS = Message.o
LIB_NAME = libmessage.so

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS) $(LIB_NAME)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(LIB_NAME): $(LIB_OBJS)
	$(CXX) -shared -o $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) $(LIB_NAME) a.out core *.o
	@echo "clean OK!"
