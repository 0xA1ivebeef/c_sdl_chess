
#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "pcg/pcg_basic.h"

#include "engine/types.h"

extern uint64_t zobrist_table[12*64];
extern uint64_t zobrist_castling[16];
extern uint64_t zobrist_enpassant[8];
extern uint64_t zobrist_black_to_move;

void init_zobrist();
uint64_t get_zobrist_hash(Position* pos);

#endif
