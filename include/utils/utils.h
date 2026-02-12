
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include "engine/types.h"

#include "utils/char_utils.h"

int abs_int(int x);
Move choose_promotion_move(Move* m);
int square_string_to_int(char* square_string);
const char* square_to_notation(int sq);

#endif 

