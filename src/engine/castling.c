
#include "include/castling.h"

int square_under_attack(int square, uint64_t atk_bb)
{
    return (atk_bb & (1ULL << square)) != 0;
}

void add_castling_move(int startsquare, int destsquare, move* legal_moves)
{
	int i = get_endindex(legal_moves);
    move m = {startsquare, destsquare, 1};
	legal_moves[i] = m;
}	

// called seperatly for kingside, queenside each (twice for one player)
int can_castle(int p, int startsquare, int destsquare, uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags, uint64_t atk_bb)
{
    /*
                qs  | K |  ks
       black:  2,  3,  4,  5,  6
       white: 58, 59, 60, 61, 62
    */
    
    // startsquare always 4 for black, 60 for white
    // destsquare = startsquare - 2 is queenside
    // destsquare = startsquare + 2 is kingside
	int min_square = min(startsquare, destsquare);
	int max_square = max(startsquare, destsquare);
	for(int i = min_square; i <= max_square; ++i)
	{
        printf("square under attack looking at square %d\n", i);
		if(square_under_attack(i, atk_bb))
        {
            printf("square %d is under attack\n", i);
            printf("according to attack bitboard: \n");
            log_bitboard(&atk_bb);
			return 0;
        }
	}

    // squares occupied by any pieces:
    // from smaller to larger square to use for loop for counting
    int startindex, endindex;
    if(startsquare < destsquare)
    {
        startindex = startsquare + 1;
        endindex = destsquare;
    }
    else
    {
        startindex = destsquare - 1;
        endindex = startsquare - 1;
    }
    for(int i = startindex; i <= endindex; ++i)
	{
		if(occupancy_bitboards[2] & (1ULL << i))
			return 0;
	}
	return 1;
}

void add_castling(int p, int square, uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags, move* legal_moves, uint64_t atk_bb)
{
	int castle_rights = game_flags[1];

    log_bitboard(&atk_bb);
    
    int destsquare;
    if(square == 4)
	{
		// black
		if(castle_rights & 1)
        {
			destsquare = 2;
            if(can_castle(p, square, destsquare, bitboards, occupancy_bitboards, game_flags, atk_bb))
                add_castling_move(square, destsquare, legal_moves);
        }
        if(castle_rights & 2)
        {
            destsquare = 6;
            if(can_castle(p, square, destsquare, bitboards, occupancy_bitboards, game_flags, atk_bb))
                add_castling_move(square, destsquare, legal_moves);
	    }
    }
	// white
	else if(square == 60)
	{	
		if(castle_rights & 4)
	    {
            destsquare = 58;
            if(can_castle(p, square, destsquare, bitboards, occupancy_bitboards, game_flags, atk_bb))
                add_castling_move(square, destsquare, legal_moves);
        }
        if(castle_rights & 8)
        {
            destsquare = 62;
            if(can_castle(p, square, destsquare, bitboards, occupancy_bitboards, game_flags, atk_bb))
                add_castling_move(square, destsquare, legal_moves);
	    }
    }
}

