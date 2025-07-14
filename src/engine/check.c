
#include "engine/check.h"

int is_check(int p, uint64_t* bitboards, uint64_t attack_bitboard)
{
	uint64_t king_bitboard = bitboards[6*p + 5];
	if(attack_bitboard & king_bitboard)
		return 1;
	return 0;
}

// only keep moves that are not check after making them on the board
void filter_moves(int p, uint64_t* bitboards, uint64_t* occupancy_bitboards, Move* legal_moves, uint64_t attack_bitboard)
{
    Move valid_moves[LEGAL_MOVES_SIZE] = {0};
    int endindex = get_endindex(legal_moves);
    int c = 0;
    uint64_t bb_copy[12] = {0};
    uint64_t occ_bb_copy[3] = {0};
    for (int i = 0; i < endindex; i++) 
    {
        // copy, drop method 
        get_bb_copy(bitboards, occupancy_bitboards, bb_copy, occ_bb_copy);
        apply_move(bb_copy, legal_moves[i].startsquare, legal_moves[i].destsquare);
        update_occupancy_bitboards(bb_copy, occ_bb_copy);
        attack_bitboard = get_attack_bitboard(!p, bb_copy, occ_bb_copy);
        if (!is_check(p, bb_copy, attack_bitboard)) 
        {
            valid_moves[c] = legal_moves[i];
            ++c;
        }
    }
    memcpy(legal_moves, valid_moves, sizeof(Move)*LEGAL_MOVES_SIZE);
    printf("valid moves: \n"); 
    log_legal_moves(legal_moves);
}


