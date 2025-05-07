
#ifndef MAIN_H
#define MAIN_H

#include "sdl_wrapper.h"
#include "game.h"
#include "renderer.h"

#include "move.h"
#include "constants.h"

typedef struct
{
    uint64_t bitboards[12];
    uint64_t occupancy[3];
    int game_flags[5];
    Move leagal_moves[LEGAL_MOVES_SIZE]
    int running;
    int needs_update;
    int selected_square;
} GameContext;

#endif
