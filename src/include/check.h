
#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>
#include <stdint.h> 
#include "legal_move_utils.h"
#include "bitboard_utils.h"

int is_check(int p, uint64_t* bitboards, uint64_t attack_bitboard);

#endif

