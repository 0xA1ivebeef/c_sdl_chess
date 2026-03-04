
#include <stdint.h>

#include "engine/types.h"

int get_king_sq(Position* pos, uint8_t player);

void get_bb_copy(uint64_t* bb, uint64_t* occ, uint64_t* bb_copy, uint64_t* occ_bb_copy);
void update_occ(Position* pos);

int count_set_bits(uint64_t num);
int get_bb_index(uint64_t* bb, int sq);
