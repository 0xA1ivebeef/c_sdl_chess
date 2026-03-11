
#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <stdint.h>

#include "engine/types.h"
#include "prng/pcg_basic.h"

uint64_t get_zobrist_hash(Position* pos);

#endif
