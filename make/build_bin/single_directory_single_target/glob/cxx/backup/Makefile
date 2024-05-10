CXX = g++
CXXFLAGS = -g3 -Wall -Wextra

SOURCES = $(shell ls *.cpp)
TARGET = Visitor

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"

