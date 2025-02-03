# Makefile for building each Cryonix utility separately

CC = cmake++
CFLAGS = -std=c++23 -Wall -Wextra -O2 -lz
BUILD_DIR = build
SRC_DIR = land-utils
BIN_DIR = bin

# Find all .cpp files in the src directory and remove the path and extension to get the program names
PROGRAMS = $(basename $(wildcard $(SRC_DIR)/*/*.cpp))

# List of source files
SOURCES = $(wildcard $(SRC_DIR)/*/*.cpp)

# Automatically create object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%/%.cpp=$(BUILD_DIR)/%.o)

# Default target is to build all programs
all: $(PROGRAMS)

# Create the binary directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Rule to build each program
$(BIN_DIR)/%: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Compile each source file into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the build and bin directories
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# List all the source files for reference
list-sources:
	@echo $(SOURCES)

# Help target to explain how to use the Makefile
help:
	@echo "Makefile for Cryonix userland"
	@echo "Usage:"
	@echo "  make all        - Build all utilities in the userland"
	@echo "  make clean      - Clean the build and bin directories"
	@echo "  make list-sources - List all source files"
	@echo "  make help       - Show this help message"

.PHONY: all clean list-sources help