
#include "include/bitboard_utils.h"

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

int is_pawn_promotion(uint64_t* bitboards, int ss, int ds)
{
    int index = get_bitboard_index(bitboards, ss);
    if(!(index == 0 || index == 6))
        return 0;
    return ((ds >= 0 && ds <= 7) || (ds >= 56 && ds <= 63));
}

void handle_pawn_promotion(uint64_t* bitboards, int ss, int ds)
{
    int ds_i = get_bitboard_index(bitboards, ds);
    bitboards[ds_i] &= ~(1ULL << ds);
    if(ds >= 0 && ds <= 7)
    {
        bitboards[6] &= ~(1ULL << ss); // delete pawn
        bitboards[10] |= (1ULL << ds); // place queen
    }   
    else
    { 
        bitboards[0] &= ~(1ULL << ss); // delete pawn
        bitboards[4] |= (1ULL << ds); // place queen
    }
}

// return bitboard index of destination square, -1 if empty
void apply_move(uint64_t* bitboards, int startsquare, int destsquare)
{
    int bitboard_index = get_bitboard_index(bitboards, startsquare);
    int dest_bitboard_index = get_bitboard_index(bitboards, destsquare);

    if(is_pawn_promotion(bitboards, startsquare, destsquare))
    {
        handle_pawn_promotion(bitboards, startsquare, destsquare);
        return;
    }
    
    bitboards[bitboard_index] &= ~(1ULL << startsquare); // delete piece on startsquare
    bitboards[bitboard_index] |= (1ULL << destsquare); // set new piece on destsquare    

    if(dest_bitboard_index == -1) // dest square empty
        return;

    bitboards[dest_bitboard_index] &= ~(1ULL << destsquare); // delete piece on destsquare
}

void update_occupancy_bitboards(uint64_t* bitboards, uint64_t* occupancy_bitboards)
{
    for(int i = 0; i < 3; ++i)
        occupancy_bitboards[i] = 0;
    
    for(int i = 0; i < 6; ++i)
        occupancy_bitboards[0] |= bitboards[i];
    
    for(int i = 6; i < 12; ++i)
        occupancy_bitboards[1] |= bitboards[i];
    
    occupancy_bitboards[2] |= occupancy_bitboards[0] | occupancy_bitboards[1];
}
