# Compiler
CXX := g++

# Output binary
TARGET := build/pma

# Source files (auto-detect all .cpp files)
SRCS := $(wildcard src/*.cpp)

# Object files
OBJS := $(patsubst src/%.cpp,build/%.o,$(SRCS))

# Flags
CXXFLAGS := -std=c++20 -Wall -Wextra -g

# raylib flags (include + link)
RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib)
RAYLIB_LIBS := $(shell pkg-config --libs raylib)

# Extra system libraries you explicitly need
SYS_LIBS := -lX11 -lm -lpthread -ldl -lrt

# Build everything
all: $(TARGET)

# Ensure build directory exists
build:
	mkdir -p build

# Link step
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(RAYLIB_LIBS) $(SYS_LIBS)

# Compile step
build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) $(RAYLIB_CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f build/*.o $(TARGET)

.PHONY: all clean