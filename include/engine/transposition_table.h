
#ifndef TRANSPOS_H
#define TRANSPOS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "engine/types.h"

typedef struct __attribute__((aligned(16)))
{
    uint64_t hash;
    Move best_move;
    uint8_t depth;
    uint8_t eval;
    uint8_t flag;
} TTEntry;

int TT_init();
void TT_store(TTEntry* new_entry);
TTEntry* TT_lookup(uint64_t hash);

#endif
