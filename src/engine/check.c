
#include "engine/check.h"

int is_check(Position* position, uint64_t attack_bitboard)
{
    // if no attack bitboard, use the one from the position
    if (attack_bitboard == 0)
    {
        int opponent = position->current_player ^ 1;
        attack_bitboard = position->attack_bitboards[opponent];
    }

    int king_sq = position->king_square[position->current_player];
	
    // Safety: ensure the square is valid
    if (king_sq < 0 || king_sq >= 64)
        return 0;

    // Use left shift to create the mask
    return (attack_bitboard & (1ULL << king_sq)) != 0;
}

void filter_moves(Position* position)
{
    Move valid_moves[LEGAL_MOVES_SIZE] = {0};
    int valid_move_count = 0;
    for (int i = 0; i < position->legal_move_count; i++)
    {
        Position copy = *position;
        Move m = position->legal_moves[i];

        apply_move(&copy, &m); // calls all special move handlings which might result in undefined behaviour if these are not stateless

        update_occupancy_bitboards(copy.bitboards, copy.occupancy);

        uint64_t attack_bitboard = get_attack_bitboard(!copy.current_player, copy.bitboards, copy.occupancy); // get enemy attack bitboard

        if (is_check(&copy, attack_bitboard))
            continue;

        valid_moves[valid_move_count++] = m;
    }
    memcpy(position->legal_moves, valid_moves, sizeof(Move) * LEGAL_MOVES_SIZE);
    position->legal_move_count = valid_move_count;
}


