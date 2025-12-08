
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include "engine/move.h"
#include "engine/pieces.h"
#include "engine/position.h"


int abs_int(int x);
Move choose_promotion_move(Move* m);
const char* square_to_notation(int sq);

#endif 

