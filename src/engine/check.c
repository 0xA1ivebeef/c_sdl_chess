
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

// only need minimal state for check detection with occupancy, attack_bitboards and king square
// maybe need some manual things cause update is not called like king square to work properly
// cause is_check relies in position->king_square which is not updates without update
// but i should not use update
// but it should be fine since the functions just get the copy and not the real position
void filter_moves(Position* position)
{
    // given the current position
    // make every possible move and see if the king is under attack
    // return only the moves that dont put the king under attack
    // -> basically cant put own king in check
    // this logic also works for moving out of checks
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
        
        // TODO either undo move or reset copy
        // currently just creating a copy every iteration which is fine
    }
    memcpy(position->legal_moves, valid_moves, sizeof(Move)*LEGAL_MOVES_SIZE);
    position->legal_move_count = valid_move_count;
}


