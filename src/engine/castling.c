
#include "engine/castling.h"

int square_under_attack(int square, uint64_t atk_bb)
{
    return (atk_bb & (1ULL << square)) != 0;
}

// remove castle_rights given index kqKQ
void remove_castle_rights(int* game_flags, int index)
{
    switch (index)
    {
        case 0:
            game_flags[1] &= 14; // keep all but black queenside
            break;
        case 1:
            game_flags[1] &= 13; // keep all but black kingside
            break;
        case 2:
            game_flags[1] &= 11; // keep all but white queenside
            break;
        case 3:
            game_flags[1] &= 7; // keep all but white kingside
            break;
        default:
            printf("CASTLING: Input Error: remove_castle_rights(int* game_flags, int index)\n");
            return;
    }
}

// castle rights = KQkq white - black
void update_castle_rights(uint64_t* bitboards, Move* this_move, int* game_flags)
{
    // if king or rook moves remove castlerights
    int index = get_bitboard_index(bitboards, this_move->startsquare);
    switch(index)
    {
        case 3: // black and white rooks
        case 9:
            // remove castle rights for one side
            switch(this_move->startsquare)
            {
                case 0:
                    remove_castle_rights(game_flags, 0); // black qs
                    break;
                case 7:
                    remove_castle_rights(game_flags, 1); // black ks
                    break;
                case 56:
                    remove_castle_rights(game_flags, 2); // white qs
                    break;
                case 63:
                    remove_castle_rights(game_flags, 3); // white ks
                    break;
                default:
                    break;
            }
            break;
        case 5: // black and white king 
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
    Move m = {startsquare, destsquare, 1};
	legal_moves[i] = m;
}	

int players_rook_on_castle_square(int startsquare, int destsquare, uint64_t* bitboards)
{
    // rook bitboards: white index 3 black index 9
    if (destsquare < startsquare) // qs 
    {
        if (startsquare == 4)
            return (bitboards[3] & (1ULL << 0)) != 0; // black qs
        else 
            return (bitboards[9] & (1ULL << 56)) != 0; // white qs
    }
    else // ks
    {
        if (startsquare == 4)
            return (bitboards[3] & (1ULL << 7)) != 0; // black ks
        else 
            return (bitboards[9] & (1ULL << 63)) != 0; // white ks
    }
}

// called seperatly for kingside, queenside each (twice for one player)
int can_castle(int startsquare, int destsquare, uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags, uint64_t atk_bb)
{
    if (destsquare == -1) 
        return 0;
    
    // update castle rights and return 0
    if (!players_rook_on_castle_square(startsquare, destsquare, bitboards))
    {
        int ks = (startsquare < destsquare);
        int index = (startsquare == 4) ? 0 : 2;
        index += ks;
        printf("CASTLING: removing castlerights with index %d\n", index);
        remove_castle_rights(game_flags, index); 
        return 0;
    }

    // startsquare always 4 for black, 60 for white
    // destsquare = startsquare - 2 is queenside
    // destsquare = startsquare + 2 is kingside
	int min_square = min(startsquare, destsquare);
	int max_square = max(startsquare, destsquare);
	for(int i = min_square; i <= max_square; ++i)
	{
        printf("CASTLING: square under attack looking at square %d\n", i);
		if(square_under_attack(i, atk_bb))
        {
            printf("CASTLING: square %d is under attack\n", i);
            printf("CASTLING: according to attack bitboard: \n");
            log_bitboard(&atk_bb);
			return 0;
        }
	}

    // squares occupied by any pieces:
    // from smaller to larger square to use for-loop for counting
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

void add_castling(int square, uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags, Move* legal_moves, uint64_t atk_bb)
{
	int castle_rights = game_flags[1];

    log_bitboard(&atk_bb);
    
    if (square == 4) 
    {
        if ((castle_rights & 1) && can_castle(4, 2, bitboards, occupancy_bitboards, game_flags, atk_bb))
            add_castling_move(4, 2, legal_moves);
        if ((castle_rights & 2) && can_castle(4, 6, bitboards, occupancy_bitboards, game_flags, atk_bb))
            add_castling_move(4, 6, legal_moves);
    } 
    else if (square == 60) 
    {
        if ((castle_rights & 4) && can_castle(60, 58, bitboards, occupancy_bitboards, game_flags, atk_bb))
            add_castling_move(60, 58, legal_moves);
        if ((castle_rights & 8) && can_castle(60, 62, bitboards, occupancy_bitboards, game_flags, atk_bb))
            add_castling_move(60, 62, legal_moves);
    }
}

