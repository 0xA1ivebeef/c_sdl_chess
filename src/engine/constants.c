
#include "engine/constants.h"

#define FILE_A 0x0101010101010101ULL
#define FILE_H 0x8080808080808080ULL

const int PAWN_VAL = 100;
const int KNIGHT_VAL = 300;
const int BISHOP_VAL = 300;
const int ROOK_VAL = 500;
const int QUEEN_VAL = 900;

const int PIECE_VALUES[5] = 
{
    PAWN_VAL,
    KNIGHT_VAL,
    BISHOP_VAL,
    ROOK_VAL,
    QUEEN_VAL
};

const int file_offsets[8] = { 1, 0, -1,  0, 1, -1,  1, -1 };
const int rank_offsets[8] = { 0, 1,  0, -1, 1,  1, -1, -1 };

const int translation_table[5] = {1, 3, 3, 5, 9};
const int depth = 3;

