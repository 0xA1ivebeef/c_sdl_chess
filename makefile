
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC_DIR = src
OBJ_DIR = obj

# Recursively find all .c files in src/
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')

# Replace src/ with obj/ and .c with .o
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Default target
all: main

# Link all object files into the final executable
main: $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -lSDL2 -lSDL2_image -o main

# Compile .c to .o, preserving directory structure
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	-rm -rf $(OBJ_DIR)
	-rm -f main core

.PHONY: all clean

