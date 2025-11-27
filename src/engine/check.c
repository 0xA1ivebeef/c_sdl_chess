
#include "engine/check.h"

int is_check(int p, uint64_t* bitboards, uint64_t attack_bitboard)
{
	uint64_t king_bitboard = bitboards[6*p + 5];
	if(attack_bitboard & king_bitboard)
		return 1;
	return 0;
}

// only keep moves that are not check after making them on the board
void filter_moves(GameContext* game, uint64_t attack_bitboard)
{
    int p = game->game_flags[0];
    Move valid_moves[LEGAL_MOVES_SIZE] = {0};
    int c = 0;
    GameContext copy = *game;
    for (int i = 0; i < game->legal_move_count; i++) 
    {
        apply_move(copy.bitboards, copy.legal_moves[i].startsquare, copy.legal_moves[i].destsquare);
        update_occupancy_bitboards(copy.bitboards, copy.occupancy);
        attack_bitboard = get_attack_bitboard(!p, copy.bitboards, copy.occupancy);
        if (!is_check(p, copy.bitboards, attack_bitboard)) 
        {
            valid_moves[c] = legal_moves[i];
            ++c;
        }
    }
    memcpy(legal_moves, valid_moves, sizeof(Move)*LEGAL_MOVES_SIZE);
}


