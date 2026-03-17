
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

#define TILESIZE 128

#define TT_EXACT 0 // exact eval
#define TT_UPPERBOUND 1 
#define TT_LOWERBOUND 2 

#define TT_SIZE (1 << 22) // ~4M entries

#define BOOK_MOVE_SIZE 1024
#define LEGAL_MOVES_SIZE 255

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

#define INVALID_SQUARE 255

#define RANK_1 0x00000000000000FFULL
#define RANK_8 0xFF00000000000000ULL

#define FILE_A 0x0101010101010101ULL
#define FILE_H 0x8080808080808080ULL

#define INF 1000000
#define MATE 3200

extern const uint16_t PROMOTION_FLAGS[4];

extern const uint16_t FROM_MASK;
extern const uint16_t TO_MASK;
extern const uint16_t PROMO_MASK; 

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
