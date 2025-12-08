
#include "engine/check.h"

int is_check(int king_square, uint64_t attack_bitboard)
{
    // return if enemy attack bitboard has set bit on own king
    return (attack_bitboard & (1ULL << king_square)) != 0;
}

void filter_moves(Position* position)
{
    Move valid_moves[LEGAL_MOVES_SIZE];
    int valid_move_count = 0;
    for (int i = 0; i < position->legal_move_count; i++)
    {
        Move m = position->legal_moves[i];
        Undo undo = {};  // store what needs to be restored

        apply_move(position, &m, &undo);          // apply move in-place
        update_occupancy_bitboards(position->bitboards, position->occupancy);

        uint64_t attack_bb = get_attack_bitboard(!position->current_player, position->bitboards, position->occupancy);

        if (!is_check(position, attack_bb))
            valid_moves[valid_move_count++] = m;

        undo_move(position, &m, &undo);           // restore position
    }
    memcpy(position->legal_moves, valid_moves, sizeof(Move) * valid_move_count);
    position->legal_move_count = valid_move_count;
}

void filter_moves(Position* position)
{
    Move valid_moves[LEGAL_MOVES_SIZE] = {0};
    int valid_move_count = 0;
    for (int i = 0; i < position->legal_move_count; i++)
    {
        Position copy = *position;
        Move m = position->legal_moves[i];

        // calls all special move handlings which might result in undefined behaviour if these are not stateless
        // make a move of white, current player is switched
        apply_move(&copy, &m); 

        update_occupancy_bitboards(copy.bitboards, copy.occupancy);

        // get enemy attack bitboard (blacks move now)
        uint64_t attack_bitboard = get_attack_bitboard(
                copy.current_player, copy.bitboards, copy.occupancy); 

        // whites king under blacks attack bitboard
        if (is_check(copy.king_square[!copy.current_player], attack_bitboard))
            continue;

        valid_moves[valid_move_count++] = m;
    }
    memcpy(position->legal_moves, valid_moves, sizeof(Move) * valid_move_count);
    position->legal_move_count = valid_move_count;
}

