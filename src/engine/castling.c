
#include "engine/castling.h"

// remove castle_rights given index kqKQ
void remove_castle_rights(int* castle_rights, int index)
{
    switch (index)
    {
        case 0:
            *castle_rights &= 14; // keep all but black queenside
            break;
        case 1:
            *castle_rights &= 13; // keep all but black kingside
            break;
        case 2:
            *castle_rights &= 11; // keep all but white queenside
            break;
        case 3:
            *castle_rights &= 7; // keep all but white kingside
            break;
        default:
            printf("CASTLING: Input Error: remove_castle_rights(int* castle_rights, int index)\n");
            return;
    }
}

// TODO optimize redo logic
// castle rights = KQkq white - black
void update_castle_rights(uint64_t* bitboards, Move* this_move, int* castle_rights)
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
                    remove_castle_rights(castle_rights, 0); // black qs
                    break;
                case 7:
                    remove_castle_rights(castle_rights, 1); // black ks
                    break;
                case 56:
                    remove_castle_rights(castle_rights, 2); // white qs
                    break;
                case 63:
                    remove_castle_rights(castle_rights, 3); // white ks
                    break;
                default:
                    break;
            }
            break;
        case 5: // black and white king 
        case 11:
            // remove castle rights for player
            *castle_rights &= (this_move->startsquare == 60) ? 3 : 12;
            break;
        default:
            return;
    }
}

// add to legal moves of position
void add_castling_move(Position* position, int startsquare, int destsquare)
{
    Move m = {startsquare, destsquare, 1};
	position->legal_moves[position->legal_move_count++] = m;
    printf("castling move added: %d, %d\n", startsquare, destsquare);
}	

int square_under_attack(int square, uint64_t attack_bitboard)
{
    return (attack_bitboard & (1ULL << square)) != 0;
}

// return if the rook is still there
int players_rook_on_castle_square(uint64_t* bitboards, int startsquare, int destsquare)
{
    // queenside
    if (destsquare < startsquare)
    {
        if (startsquare == 4) // black
            return (bitboards[BLACK_ROOK] & (1ULL << 0)) != 0; 
        else // white
            return (bitboards[WHITE_ROOK] & (1ULL << 56)) != 0; 
    }
    // kingside
    else 
    {
        if (startsquare == 4) // black
            return (bitboards[BLACK_ROOK] & (1ULL << 7)) != 0;
        else // white
            return (bitboards[BLACK_ROOK] & (1ULL << 63)) != 0; 
    }
}

// TODO seperate logic in single functions
// called seperatly for kingside, queenside each (twice for one player)
// can_castle should not mutate castle_rights !! 
int can_castle(Position* position, int startsquare, int destsquare)
{
    printf("can castle called with: %s = %d, %s = %d\n", square_to_notation(startsquare), startsquare, square_to_notation(destsquare), destsquare); 

    if (destsquare == -1) 
        return 0;
    
    if (!players_rook_on_castle_square(position->bitboards, startsquare, destsquare))
        return 0;

    // startsquare always 4 for black, 60 for white
    // destsquare = startsquare - 2 is queenside
    // destsquare = startsquare + 2 is kingside
	int min_square = min(startsquare, destsquare);
	int max_square = max(startsquare, destsquare);
    uint64_t enemy_attack_bitboard = position->attack_bitboards[!position->current_player];
	for(int i = min_square; i <= max_square; ++i)
	{
        printf("CASTLING: looking at square %d\n", i);
		if(square_under_attack(i, enemy_attack_bitboard))
        {
            printf("CASTLING: square %d is under attack\n", i);
            printf("CASTLING: according to attack bitboard: \n");
            log_bitboard(&enemy_attack_bitboard);
			return 0;
        }
	}

    printf("\n");

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
		if(position->occupancy[2] & (1ULL << i))
			return 0;
	}

	return 1;
}

// TODO: this is weird logic because you might have to take multiple paths which makes
// simplifying hard, how can you optimize this?
// given king square and position, add castling moves to legal moves if they are legal
void add_castling(Position* position)
{
	int castle_rights = position->castle_rights;

    if (position->king_square[position->current_player] == 4) 
    {
        if ((castle_rights & 1) && can_castle(position, 4, 2))
            add_castling_move(position, 4, 2);
        if ((castle_rights & 2) && can_castle(position, 4, 6))
            add_castling_move(position, 4, 6);
    } 
    else if (position->king_square[position->current_player] == 60) 
    {
        if ((castle_rights & 4) && can_castle(position, 60, 58))
            add_castling_move(position, 60, 58);
        if ((castle_rights & 8) && can_castle(position, 60, 62))
            add_castling_move(position, 60, 62);
    }
}

