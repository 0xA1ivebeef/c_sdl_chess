
#include "utils/bitboard_utils.h"

void get_bb_copy(uint64_t* bitboards, uint64_t* occupancy_bitboards, uint64_t* bb_copy, uint64_t* occ_bb_copy)
{
    for(int i = 0; i < 12; ++i)
        bb_copy[i] = bitboards[i];
    for(int i = 0; i < 3; ++i)
        occ_bb_copy[i] = occupancy_bitboards[i];
}

int get_bitboard_index(uint64_t* bitboards, int square)
{
    for(int i = 0; i < 12; ++i)
    {
        if(bitboards[i] & (1ULL << square))
            return i;
    }
    return -1;
}

int count_set_bits(uint64_t num) 
{
    int c = 0;
    while(num) 
    {
        num &= (num - 1);
        ++c;
    }
    return c;
}

void update_occupancy_bitboards(uint64_t* bitboards, uint64_t* occupancy_bitboards)
{
    // clear
    for(int i = 0; i < 3; ++i)
        occupancy_bitboards[i] = 0;
    
    // black pieces
    for(int i = 0; i < 6; ++i)
        occupancy_bitboards[BLACK] |= bitboards[i];
 
    // white pieces
    for(int i = 6; i < 12; ++i)
        occupancy_bitboards[WHITE] |= bitboards[i];
    
    // occ
    occupancy_bitboards[2] |= occupancy_bitboards[BLACK] | occupancy_bitboards[WHITE];
}
