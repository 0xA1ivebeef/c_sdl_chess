
#include "include/attack_generator.h"

uint64_t get_pawn_attack_moves(int p, int square, uint64_t* occupancy_bitboards)
{
	return bitmasks[5 - p][square] & occupancy_bitboards[!p];
}

uint64_t get_knight_attack_moves(int p, int square, uint64_t* occupancy_bitboards)
{
    return (bitmasks[6][square] & ~occupancy_bitboards[p]);
}

void atk_resolve_direction(int p, int square, int dir, uint64_t* result, uint64_t* occupancy_bitboards)
{
    int file = square % 8;
    int rank = square / 8;
    
    int this_file, this_rank, attacking_square;

    for (int i = 1; i < 8; ++i)
    {
        this_file = file + i * file_offsets[dir];
        this_rank = rank + i * rank_offsets[dir];

        if (this_file < 0 || this_file > 7 || this_rank < 0 || this_rank > 7)
            return;

        attacking_square = this_rank * 8 + this_file;

        if (occupancy_bitboards[!p] & (1ULL << attacking_square))
        {
            *result |= (1ULL << attacking_square);
            return;
        }
        else if (occupancy_bitboards[p] & (1ULL << attacking_square))
            return;
        else
            *result |= (1ULL << attacking_square);
    }
}

uint64_t get_sliding_piece_attack_moves(int p, int square, int bb_index, uint64_t* occupancy_bitboards)
{
    uint64_t result = 0;

    int startindex = (bb_index == 2 ? 4 : 0);
    int endindex = (bb_index == 3 ? 4 : 8);

    for (int dir = startindex; dir < endindex; ++dir)
        atk_resolve_direction(p, square, dir, &result, occupancy_bitboards);

    return result;
}

uint64_t get_king_attack_moves(int p, int square, uint64_t* occupancy_bitboards)
{
    return (bitmasks[10][square] & ~occupancy_bitboards[p]);
}

uint64_t get_attack_moves_bitmask(int p, int square, int bb_index, uint64_t* occupancy_bitboards)
{
	bb_index %= 6;
	switch(bb_index)
	{
		case 0:
			return get_pawn_attack_moves(p, square, occupancy_bitboards);
		case 1:
			return get_knight_attack_moves(p, square, occupancy_bitboards);
		case 2:
		case 3:
		case 4:
			return get_sliding_piece_attack_moves(p, square, bb_index, occupancy_bitboards);
		case 5:
			return get_king_attack_moves(p, square, occupancy_bitboards);
		default:
			return 0;
	}
}

uint64_t resolve_attack_bitboard(int p, uint64_t bb, int bb_index, uint64_t* occupancy_bitboards)
{
	uint64_t result = 0;
	int square = 0;
	while(bb)
	{
        if(bb & 1)
		    result |= get_attack_moves_bitmask(p, square, bb_index, occupancy_bitboards);
		bb >>= 1;
		++square;
	}
	return result;
}

uint64_t get_attack_bitboard(int p, uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags)
{
	uint64_t result = 0;
	int bb_startindex = p*6;
	for(int i = bb_startindex; i < bb_startindex + 6; ++i)
        result |= resolve_attack_bitboard(p, bitboards[i], i, occupancy_bitboards);; 
    // result |= square of game_flags[2] which is enpassant
	
    return result;
}
