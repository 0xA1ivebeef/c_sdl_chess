
#include <stdint.h>

void get_bb_copy(uint64_t* bitboards, uint64_t* occupancy_bitboards, uint64_t* bb_copy, uint64_t* occ_bb_copy);
int get_bitboard_index(uint64_t* bitboards, int square);
int count_set_bits(uint64_t num);
void apply_move(uint64_t* bitboards, int startsquare, int destsquare);
void update_occupancy_bitboards(uint64_t* bitboards, uint64_t* occupancy_bitboards);
