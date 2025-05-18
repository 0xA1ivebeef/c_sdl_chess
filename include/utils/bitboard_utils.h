
#include <stdint.h>

void get_bb_copy(uint64_t* bitboards, uint64_t* occupancy_bitboards, uint64_t* bb_copy, uint64_t* occ_bb_copy);
void update_occupancy_bitboards(uint64_t* bitboards, uint64_t* occupancy_bitboards);
int count_set_bits(uint64_t num);
int get_bitboard_index(uint64_t* bitboards, int square);
