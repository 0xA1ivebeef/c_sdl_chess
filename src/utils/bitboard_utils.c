
#include "utils/bitboard_utils.h"

int get_king_square(uint64_t king_bb)
{
    int i = 0;
    while(king_bb)
    {
        king_bb >>= 1;
        ++i;
    }
    return i - 1;
}

void get_bb_copy(uint64_t* bb, uint64_t* occ, uint64_t* bb_copy, uint64_t* occ_bb_copy)
{
    for(int i = 0; i < 12; ++i)
        bb_copy[i] = bb[i];

    for(int i = 0; i < 3; ++i)
        occ_bb_copy[i] = occ[i];
}

int get_bb_index(uint64_t* bb, int sq)
{
    for(int i = 0; i < 12; ++i)
    {
        if(bb[i] & (1ULL << sq))
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

void update_occ(uint64_t* bb, uint64_t* occ)
{
    // clear
    for(int i = 0; i < 3; ++i)
        occ[i] = 0;
    
    // black pieces
    for(int i = 0; i < 6; ++i)
        occ[BLACK] |= bb[i];
 
    // white pieces
    for(int i = 6; i < 12; ++i)
        occ[WHITE] |= bb[i];
    
    // occ
    occ[2] |= occ[BLACK] | occ[WHITE];
}

