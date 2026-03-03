
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

#define TILESIZE 128

#define LEGAL_MOVES_SIZE 255

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

#define INVALID_SQUARE 255

#define RANK_1 0x00000000000000FFULL
#define RANK_8 0xFF00000000000000ULL

extern const int file_offsets[8];
extern const int rank_offsets[8];
extern const int translation_table[5];

#endif
