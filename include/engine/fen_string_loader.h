
#ifndef FEN_STRING_LOADER_H
#define FEN_STRING_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "engine/types.h"

#include "utils/utils.h"
#include "utils/char_utils.h"

#define FEN_STRING_COUNT 6

extern const int FEN_STRING_MOVE_COUNT_DEPTH_5[FEN_STRING_COUNT];
extern const char* FEN_STRINGS[FEN_STRING_COUNT];

void load_fen_string(Position* pos, int index);

#endif
