# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++23 -Wall -Wextra -O2
LDFLAGS = -lz

# Directories
BUILD_DIR = build
SRC_DIR = land-utils
BIN_DIR = bin

# File lists
SOURCES = $(wildcard $(SRC_DIR)/*/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
EXECUTABLES = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%,$(SOURCES))

# Default target
all: $(EXECUTABLES)

# Build rule for executables
$(BIN_DIR)/%: $(BUILD_DIR)/%.o
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Build rule for object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean