CXX = g++
CXXFLAGS = -g3 -Wall -Wextra

SOURCES = Main.cpp Directory.cpp Element.cpp Entry.cpp File.cpp ListVisitor.cpp Visitor.cpp
TARGET = Visitor

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"

