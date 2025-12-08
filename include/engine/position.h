
#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <stdint.h>

#include "engine/move.h"
#include "engine/constants.h"

typedef struct
{
    uint64_t bitboards[12];
    uint64_t occupancy[3];

    int current_player;
    int castle_rights;
    int enpassant_square;
    int halfmove_clock;
    int fullmove_number;

    Move legal_moves[LEGAL_MOVES_SIZE];
    int legal_move_count;

    uint64_t attack_bitboards[2];

    int king_square[2];
} Position;

typedef struct
{
    int ss_bb_i;
    int ds_bb_i;
    int castle_rights;
    int enpassant_square;
    int halfmove_clock;
    int fullmove_number;
    int promote_bitboard;
    int king_square[2];
} Undo;

#endif

