CXX = g++
CXXFLAGS = -g3 -Wall -Wextra

INCLUDES = -Ilib
LDFLAGS = -Llib
LDLIBS = -lmessage

OBJECTS = hello-world.o
TARGET = hello-world

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS)
	$(MAKE) --directory=lib
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDES)

.PHONY:
clean:
	$(MAKE) clean --directory=lib
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"
