
#ifndef BITMASK_LOADER_H
#define BITMASK_LOADER_H

#include <stdint.h>
#include "constants.h"

extern uint64_t bitmasks[11][64]; 

// 0  white pawn normal
// 1  black pawn normal
// 2  white pawn double
// 3  black pawn double
// 4  white pawn capture
// 5  black pawn capture
// 6  knight
// 7  bishop
// 8  rook
// 9  queen
// 10 king  

void load_bitmasks();

#endif
