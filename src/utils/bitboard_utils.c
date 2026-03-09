
#include "utils/bitboard_utils.h"

int get_king_sq(Position* pos, uint8_t player)
{
    uint64_t bb = pos->bb[player ? WHITE_KING : BLACK_KING];
    return bb ? __builtin_ctzll(bb) : -1;
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

void update_occ(Position* pos)
{
    // clear
    for(int i = 0; i < 3; ++i)
        pos->occ[i] = 0;
    
    // black pieces
    for(int i = 0; i < 6; ++i)
        pos->occ[BLACK] |= pos->bb[i];
 
    // white pieces
    for(int i = 6; i < 12; ++i)
        pos->occ[WHITE] |= pos->bb[i];
    
    // occ
    pos->occ[2] |= pos->occ[BLACK] | pos->occ[WHITE];
}

