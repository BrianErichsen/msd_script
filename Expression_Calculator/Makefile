# Compiler and compiler flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Executable's name
TARGET = msdscript

# Source files and object files
SRCS = msdscript.cpp cmdline.cpp expr.cpp parse.cpp test.cpp val.cpp
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

documentation: Doxyfile
	doxygen $<

test_msd: test_msdscript.cpp exec.cpp expr.h
	$(CXX) $(CXXFLAGS) test_msdscript.cpp exec.cpp -o test_msd