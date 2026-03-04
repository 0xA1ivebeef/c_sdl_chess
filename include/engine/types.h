
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

typedef struct 
{
    int running;
    uint8_t selected_square;
    int needs_update;
    int game_over;
} UIContext;

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

    uint8_t player;
    uint8_t castle_rights;
    uint8_t enpassant;
    uint8_t halfmove;
    uint16_t fullmove;

    Move legal_moves[LEGAL_MOVES_SIZE];
    uint8_t legal_move_count;
} Position;

typedef struct
{
    int ss_bb_i;
    int ds_bb_i;
    uint8_t castle_rights;
    uint8_t enpassant;
    uint8_t halfmove;
    uint16_t fullmove;
    int promote_bb_i;
    uint8_t valid; 
} Undo;

#endif

