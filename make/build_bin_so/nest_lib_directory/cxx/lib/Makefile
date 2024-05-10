CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -fPIC
LDFLAGS = 

OBJECTS = Message.o
TARGET = libmessage.so

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS)
	$(CXX) -shared -o $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"
