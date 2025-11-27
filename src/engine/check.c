
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
    int current_player = position->game_flags[0];
    uint64_t attack_bitboard = 0;
    Move valid_moves[LEGAL_MOVES_SIZE] = {0};
    int c = 0;
    Position copy = *position;
    for (int i = 0; i < position->legal_move_count; i++)
    {
        apply_move(copy.bitboards, copy.legal_moves[i].startsquare, copy.legal_moves[i].destsquare);
        update_occupancy_bitboards(copy.bitboards, copy.occupancy);
        attack_bitboard = position->attack_bitboards[!current_player];
        if (!is_check(current_player, copy.bitboards, attack_bitboard)) 
        {
            valid_moves[c] = position->legal_moves[i];
            ++c;
        }
    }
    memcpy(position->legal_moves, valid_moves, sizeof(Move)*LEGAL_MOVES_SIZE);
}


