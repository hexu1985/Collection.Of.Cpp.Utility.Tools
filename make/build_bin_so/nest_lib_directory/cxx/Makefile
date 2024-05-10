CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -fPIC

INCLUDES = -Ilib
LDFLAGS = -Llib -Wl,-rpath,'$$ORIGIN/lib' -Wl,--enable-new-dtags 
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
