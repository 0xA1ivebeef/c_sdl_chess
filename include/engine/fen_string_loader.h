
#ifndef FEN_STRING_LOADER_H
#define FEN_STRING_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "engine/position.h"

#include "utils/char_utils.h"

void load_fen_string(Position* position);

#endif
