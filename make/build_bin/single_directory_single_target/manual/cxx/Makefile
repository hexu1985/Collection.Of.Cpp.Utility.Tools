CXX = g++
CXXFLAGS = -g3 -Wall -Wextra

OBJECTS = Main.o Directory.o Element.o Entry.o File.o ListVisitor.o Visitor.o
TARGET = Visitor

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"
