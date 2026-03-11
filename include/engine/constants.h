
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


extern const int PAWN_VAL;
extern const int KNIGHT_VAL;
extern const int BISHOP_VAL;
extern const int ROOK_VAL;
extern const int QUEEN_VAL;

extern const int PIECE_VALUES[5];

extern const int file_offsets[8];
extern const int rank_offsets[8];
extern const int translation_table[5];

#endif
