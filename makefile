
CC = gcc
CFLAGS = -O3 -march=native -flto -Wall -Wextra -g -Iinclude

SRC_DIR = src
OBJ_DIR = obj

# Recursively find all .c files in src/
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')

# Convert .c file list to .o file list, putting .o files under obj/
OBJ_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Default target
all: main

# Link all object files into the final executable
main: $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -lSDL2 -lSDL2_image -o main

# Compile .c to .o, preserving directory structure inside obj/
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	-rm -rf $(OBJ_DIR)
	-rm -f main core

.PHONY: all clean

