
CC = gcc
CFLAGS = -Wall -Wextra -g

SRC_FILES = $(wildcard src/*.c)
OBJ_FILES = $(patsubst src/%.c, obj/%.o, $(SRC_FILES))

all: main

main: $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -lSDL2 -lSDL2_image -o main

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm obj/*
	-rm -f main
	-rm -f core
.PHONY: all clean
