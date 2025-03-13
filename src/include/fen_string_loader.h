
#ifndef FEN_STRING_LOADER_H
#define FEN_STRING_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "char_utils.h"

void load_fen_string(uint64_t* bitboards, int* game_flags);

#endif
