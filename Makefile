# Makefile for building all Cryonix utilities

CC = clang++
CFLAGS = -std=c++23 -Wall -Wextra -O2
BUILD_DIR = build
SRC_DIR = land-utils
BIN_DIR = bin
EXEC_NAME = cryonix_userland

# List all your source files here
SOURCES = $(wildcard $(SRC_DIR)/*/*.cpp)

# Automatically create object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%/%.cpp=$(BUILD_DIR)/%.o)

# Final executable name
TARGET = $(BIN_DIR)/$(EXEC_NAME)

# Default target
all: $(TARGET)

# Create the binary directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Link the object files to create the final executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compile each source file into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the build directory
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