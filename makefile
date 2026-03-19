
CC = gcc

SRC_DIR = src
OBJ_DIR = obj

SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
OBJ_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

BUILD ?= debug  # default is debug

ifeq ($(BUILD),debug)
    CFLAGS = -g -O1 -fsanitize=address,undefined -Wall -Wextra -Iinclude
    LDFLAGS = -fsanitize=address,undefined -lm -lSDL2 -lSDL2_image -lSDL2_mixer
else ifeq ($(BUILD),release)
    CFLAGS = -O3 -march=native -flto -Wall -Wextra -Iinclude
    LDFLAGS = -lm -lSDL2 -lSDL2_image -lSDL2_mixer
else
    $(error Unknown BUILD type "$(BUILD)")
endif

all: main

main: $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(LDFLAGS) -o main

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -rf $(OBJ_DIR)
	-rm -f main core

.PHONY: all clean
