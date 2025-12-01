
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
    Position copy = *position;

    Move valid_moves[LEGAL_MOVES_SIZE] = {0};
    int valid_moves_counter = 0;
    for (int i = 0; i < position->legal_move_count; i++)
    {
        apply_move(copy.bitboards, copy.legal_moves[i].startsquare, copy.legal_moves[i].destsquare); // special moves are part of legal moves but apply moves cant apply special moves so the calculation will fail
        update_occupancy_bitboards(copy.bitboards, copy.occupancy);
        attack_bitboard = position->attack_bitboards[!current_player];
        if (!is_check(current_player, copy.bitboards, attack_bitboard)) 
        {
            valid_moves[valid_moves_counter] = position->legal_moves[i];
            ++valid_moves_counter;
        }
    }
    memcpy(position->legal_moves, valid_moves, sizeof(Move)*LEGAL_MOVES_SIZE);
}


