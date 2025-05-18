
#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <stdint.h>

#include "engine/move.h"
#include "engine/constants.h"

typedef struct
{
    uint64_t bitboards[12];
    uint64_t occupancy[3];
    int game_flags[5];
    Move legal_moves[LEGAL_MOVES_SIZE];
    int running;
    int needs_update;
    int selected_square;
} GameContext;

#endif

