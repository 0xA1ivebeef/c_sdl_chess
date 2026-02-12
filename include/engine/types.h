
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include "engine/constants.h"

#define BLACK 0
#define WHITE 1

typedef enum Piece
{
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING,
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    PIECE_COUNT
} Piece;

typedef enum Moveflags
{
    NORMAL_FLAG,
    CASTLE_FLAG,
    ENPASSANT_FLAG, 
    KNIGHT_PROMOTION, 
    BISHOP_PROMOTION,
    ROOK_PROMOTION,
    QUEEN_PROMOTION,
    DOUBLE_PAWN_PUSH, 
} Moveflags;

typedef struct Move
{
    uint8_t start;
    uint8_t dest;
    uint8_t flags;
} Move;

typedef struct
{
    uint64_t bb[12];
    uint64_t occ[3];

    int player;
    int castle_rights;
    int enpassant;
    int halfmove;
    int fullmove;

    Move legal_moves[LEGAL_MOVES_SIZE];
    int legal_move_count;

    int king_sq[2];
} Position;

typedef struct
{
    int ss_bb_i;
    int ds_bb_i;
    int castle_rights;
    int enpassant;
    int halfmove;
    int fullmove;
    int promote_bb;
} Undo;

#endif

