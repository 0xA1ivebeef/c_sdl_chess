
#include "engine/check.h"

int is_check(int player, uint64_t* bitboards, uint64_t attack_bitboard)
{
	uint64_t king_bitboard = bitboards[6*player + 5];
	if(attack_bitboard & king_bitboard)
		return 1;
	return 0;
}

void filter_moves(Position* position)
{
    // given the current position
    // make every possible move and see if the king is under attack
    // return only the moves that dont put the king under attack
    // -> basically cant put own king in check
    // this logic also works for moving out of checks

    Move valid_moves[LEGAL_MOVES_SIZE] = {0};
    int valid_moves_counter = 0;
    for (int i = 0; i < position->legal_move_count; i++)
    {
        Position copy = *position;
        Move m = position->legal_moves[i];

        apply_move(copy, &m);

        update_occupancy_bitboards(copy.bitboards, copy.occupancy);

        uint64_t attack_bitboard = get_attack_bitboard(!copy.current_player, copy.bitboards, copy.occupancy); // get enemy attack bitboard
        if (!is_check(copy.current_player, copy.bitboards, attack_bitboard)) 
        {
            valid_moves[valid_move_count++] = m;
        }
        // either undo move or reset copy
    }
    memcpy(position->legal_moves, valid_moves, sizeof(Move)*LEGAL_MOVES_SIZE);
    position->legal_move_count = valid_move_count;
}


