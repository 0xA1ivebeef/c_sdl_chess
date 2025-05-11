
#include "include/castling.h"

int square_under_attack(int square, uint64_t atk_bb)
{
    return (atk_bb & (1ULL << square)) != 0;
}

// castle rights = KQkq white - black
void update_castle_rights(uint64_t* bitboards, Move* this_move, int* game_flags)
{
    int index = get_bitboard_index(bitboards, this_move->startsquare);
    switch(index)
    {
        case 3:
        case 9:
            // remove castle rights for one side
            switch(this_move->startsquare)
            {
                case 0:
                    game_flags[1] &= 14; // keep all but black queenside
                    break;
                case 7:
                    game_flags[1] &= 13; // keep all but black kingside
                    break;
                case 56:
                    game_flags[1] &= 11; // white qs
                    break;
                case 63:
                    game_flags[1] &= 7; // white kingside
                    break;
                default:
                    break;
            }
            break;
        case 5:
        case 11:
            // remove castle rights for player
            game_flags[1] &= (this_move->startsquare == 60) ? 3 : 12;
            break;
        default:
            return;
    }
}

void handle_castling(uint64_t* bitboards, Move* this_move, int* game_flags)
{
    int startsquare = this_move->startsquare;
    int destsquare = this_move->destsquare;
    int bb_index = (startsquare == 60) ? 9 : 3;
    if(startsquare > destsquare)
    {
        bitboards[bb_index] &= ~(1ULL << (destsquare - 2));
        bitboards[bb_index] |= (1ULL << (destsquare + 1));
    }
    else
    {
        bitboards[bb_index] &= ~(1ULL << (destsquare + 1));
        bitboards[bb_index] |= (1ULL << (destsquare - 1));
    }
    game_flags[1] &= (startsquare == 60) ? 3 : 12;
}



void add_castling_move(int startsquare, int destsquare, Move* legal_moves)
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

void add_castling(int p, int square, uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags, Move* legal_moves, uint64_t atk_bb)
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

