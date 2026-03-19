
#ifndef TRANSPOS_H
#define TRANSPOS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "engine/types.h"

int TT_init();
void TT_clear();
void TT_store(TTEntry* new_entry);
TTEntry* TT_lookup(uint64_t hash);

#endif
