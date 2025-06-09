#command: mingw32-make

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11

# Executable target
TARGET = petshelter.exe

# Source files
SRCS = petshelter.cpp pets.cpp cat.cpp dog.cpp InterestedAdopter.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default: build and run
all: $(TARGET) run

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the program
run: $(TARGET)
	$(TARGET)

# Clean up generated files
clean:
	del /Q $(TARGET) $(OBJS) 2>nul || exit 0
