
#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "pcg/pcg_basic.h"

#include "engine/types.h"

void init_zobrist();
uint64_t get_zobrist_hash(Position* pos);

#endif
