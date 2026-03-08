
#include "engine/castling.h"

#define BLACK_CASTLE_RIGHTS  3 // 0011b
#define WHITE_CASTLE_RIGHTS 12 // 1100b

// index 0 = BLACK_QUEENSIDE, 1 = BLACK_KINGSIDE, 2 = WHITE_QUEENSIDE, 3 = WHITE_KINGSIDE
const int castle_starts[4] = {4, 4, 60, 60};   // e8, e8, e1, e1
const int castle_dests[4]  = {2, 6, 58, 62};   // c8, g8, c1, g1

const int ROOK_SQUARES[4] = {0, 7, 56, 63};

const uint64_t BLACK_QUEENSIDE_MASK = (1ULL << 4) | (1ULL << 3) | (1ULL << 2);
const uint64_t BLACK_KINGSIDE_MASK  = (1ULL << 4) | (1ULL << 5) | (1ULL << 6);

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

void update_castle_rights(Position* pos, Move* m)
{
    // if white castled remove whites castle rights
    if (m->flags == CASTLE_FLAG)
    {
        pos->castle_rights &= (pos->player == WHITE) 
            ? ~WHITE_CASTLE_RIGHTS 
            : ~BLACK_CASTLE_RIGHTS;
    }

    // king moves -> remove both sides
    if (m->start == 4)
    {
        pos->castle_rights &= ~BLACK_CASTLE_RIGHTS;
    }
    else if (m->start == 60)
    {
        pos->castle_rights &= ~WHITE_CASTLE_RIGHTS;
    }

	for (int i = 0; i < 4; ++i)
	{
		if(m->start == ROOK_SQUARES[i] || m->dest == ROOK_SQUARES[i]) 
    		pos->castle_rights &= ~(1 << i);
	}
}

// called seperatly for kingside, queenside each (twice for one player)
int can_castle(Position* pos, int bit_index)
{
	uint64_t bitmask = CASTLING_MASKS[bit_index];

    // squares under attack
	uint64_t enemy_attack_bb = get_attack_bb(pos, !pos->player);
	if (enemy_attack_bb & bitmask)
	{
		// printf("CAN CASTLE square under attack\n");
		return 0;	
	}

	// correct the bitmask to handle occ
	bitmask &= ~((1ULL << 4) | (1ULL << 60));
	if (pos->occ[2] & bitmask) 
	{
		// printf("CAN CASTLE square is occupied\n");
		return 0;
	}

	return 1;
}

// given pos, add castling moves to legal moves if they are legal
void add_castling(Position* pos)
{
    for(int side = 0; side < 2; ++side) 
    { // 0=queenside, 1=kingside
        int bit_index = pos->player * 2 + side;  // maps to 0..3
        uint8_t castle_flag = 1 << bit_index; // 1 << 0..3 = 1, 2, 4, 8

		// printf("checking for castle_rights\n");
        if(pos->castle_rights & castle_flag) 
        {
			// printf("checking if can castle\n");
            if(can_castle(pos, bit_index)) // passing bit_index 0..3 to map to bitmasks
            {        
				// printf("adding castling move\n");
                Move m = { castle_starts[bit_index], castle_dests[bit_index], CASTLE_FLAG };
                pos->legal_moves[pos->legal_move_count++] = m;
            }   
        }
    }
}

