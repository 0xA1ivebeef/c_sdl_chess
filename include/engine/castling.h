
#ifndef CASTLING_H
#define CASTLING_H

#include <stdio.h>

#include "engine/position.h"
#include "engine/constants.h"
#include "engine/move.h"

void add_castling(Position* position);
void update_castle_rights(uint64_t* bitboards, Move* this_move, int* castle_rights);

#endif


