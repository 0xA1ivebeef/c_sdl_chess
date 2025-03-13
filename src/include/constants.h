
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024
#define TILESIZE 128

#define LEGAL_MOVES_SIZE 256

extern const int file_offsets[8];
extern const int rank_offsets[8];
extern const int translation_table[5];

int is_on_board(int file, int rank);

#endif
