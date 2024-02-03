# Compiler and compiler flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Executable's name
TARGET = msdscript

# Source files and object files
SRCS = msdscript.cpp cmdline.cpp expr.cpp test_expr.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove generated files
.PHONY: clean
clean:
	rm -rf $(OBJS) $(TARGET)

# Run the exe
run: $(TARGET)
	./$(TARGET)