# Compiler settings
CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
# Update the pattern for executables to have .out extension and exclude specific executables
EXECUTABLES = $(filter-out $(BIN_DIR)/read-data.out $(BIN_DIR)/matrix.out, $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.out, $(SOURCES)))

# Default target
all: $(BIN_DIR) $(EXECUTABLES)

# Compile each source file to an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link each object file to an executable, excluding read-data.o and matrix.o, and add .out extension
$(BIN_DIR)/%.out: $(OBJ_DIR)/%.o $(OBJ_DIR)/matrix.o $(OBJ_DIR)/read-data.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Create bin and obj directories if they don't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
