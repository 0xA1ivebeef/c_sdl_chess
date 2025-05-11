
#ifndef MOVE_HANDLER_H
#define MOVE_HANDLER_H

#include <stdint.h>

#include "game/context.h"
#include "engine/move.h"

int is_legal_move(uint64_t* bitboards, int startsquare, int destsquare, int* game_flags, Move* legal_moves);
int handle_move(uint64_t* bitboards, int startsquare, int destsquare, Move* legal_moves, int* game_flags;)

#endif
