
#include "engine/castling.h"

// index 0 = BLACK_QUEENSIDE, 1 = BLACK_KINGSIDE, 2 = WHITE_QUEENSIDE, 3 = WHITE_KINGSIDE
const int castle_starts[4] = {4, 4, 60, 60};   // e8, e8, e1, e1
const int castle_dests[4]  = {2, 6, 58, 62};   // c8, g8, c1, g1

const int ROOK_SQUARES[4] = {0, 7, 56, 63};

// Black
const uint64_t BLACK_QUEENSIDE_MASK = (1ULL << 4) | (1ULL << 3) | (1ULL << 2);
const uint64_t BLACK_KINGSIDE_MASK  = (1ULL << 4) | (1ULL << 5) | (1ULL << 6);

// White
const uint64_t WHITE_QUEENSIDE_MASK = (1ULL << 60) | (1ULL << 59) | (1ULL << 58);
const uint64_t WHITE_KINGSIDE_MASK  = (1ULL << 60) | (1ULL << 61) | (1ULL << 62);

const uint64_t CASTLING_MASKS[4] = 
{
	BLACK_QUEENSIDE_MASK,
	BLACK_KINGSIDE_MASK,
	WHITE_QUEENSIDE_MASK,
	WHITE_KINGSIDE_MASK
};

const char* CASTLE_STRINGS[4] =
{
	"BLACK_QUEENSIDE",
	"BLACK_KINGSIDE",
	"WHITE_QUEENSIDE",
	"WHITE_KINGSIDE"	
};

void update_castle_rights(Position* position, Move* this_move)
{
	// king moves -> remove both sides
	if (this_move->startsquare == 4) // black king
	{
    	position->castle_rights &= ~((1ULL << 0) | (1ULL << 1)); // clear BLACK_QS and BLACK_KS
		// printf("UPDATE CASTLE RIGHTS removing black castling\n");
	}

	if (this_move->startsquare == 60) // white king
	{
    	position->castle_rights &= ~((1ULL << 2) | (1ULL << 3)); // clear WHITE_QS and WHITE_KS
		// printf("UPDATE CASTLE RIGHTS removing white castling\n");
	}

	for (int i = 0; i < 4; ++i)
	{
		if(this_move->startsquare == ROOK_SQUARES[i] || this_move->destsquare == ROOK_SQUARES[i]) 
		{
			// printf("removing castle rights for %s\n", CASTLE_STRINGS[i]);
    		position->castle_rights &= ~(1 << i);
		}
	}
}

// add to legal moves of position
void add_castling_move(Position* position, int startsquare, int destsquare)
{
    Move m = {startsquare, destsquare, 1};
	position->legal_moves[position->legal_move_count++] = m;
    // printf("castling move added: %d, %d\n", startsquare, destsquare);
}	

// called seperatly for kingside, queenside each (twice for one player)
int can_castle(Position* position, int bit_index)
{
	uint64_t bitmask = CASTLING_MASKS[bit_index];

    // squares under attack
	if (position->attack_bitboards[!position->current_player] & bitmask)
		return 0;	

	// correct the bitmask to handle occupancy
	bitmask &= ~((1ULL << 4) | (1ULL << 60));
	if (position->occupancy[2] & bitmask) 
		return 0;

	return 1;
}

// given position, add castling moves to legal moves if they are legal
void add_castling(Position* position)
{
    // only add for current player
    // do both sides of course k and q
	int castle_rights = position->castle_rights;
    int current_player = position->current_player;
   
    for(int side = 0; side < 2; ++side) 
    { // 0=queenside, 1=kingside
        int bit_index = current_player * 2 + side;  // maps to 0..3
        uint8_t castle_flag = 1 << bit_index; // 1 << 0..3 = 1, 2, 4, 8

        if(castle_rights & castle_flag) 
        {
            if(can_castle(position, bit_index)) // passing bit_index 0..3 to map to bitmasks
            {        
                add_castling_move(position, castle_starts[bit_index], castle_dests[bit_index]);
            }   
        }
    }
}

