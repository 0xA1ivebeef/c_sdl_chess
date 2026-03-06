
#include "engine/check.h"

int square_under_attack(uint8_t sq, uint64_t attack_bb)
{
    return ((attack_bb & (1ULL << sq)) != 0);
}

int is_check(int king_sq, uint64_t attack_bb)
{
    return square_under_attack(king_sq, attack_bb);
}

void filter_moves(Position* pos)
{
    Move valid_moves[LEGAL_MOVES_SIZE] = {0};
    int valid_move_count = 0;

    for (int i = 0; i < pos->legal_move_count; i++)
    {
        Undo undo = {0};  
        Move m = pos->legal_moves[i];

        save_state(pos, &m, &undo);
        apply_move(pos, &m);  

        uint8_t king_sq = get_king_sq(pos, !pos->player);
        uint64_t attack_bb = get_attack_bb(pos, pos->player);
        if (!square_under_attack(king_sq, attack_bb))
            valid_moves[valid_move_count++] = m;

        undo_move(pos, &m, &undo); // restore position
    }

    memcpy(pos->legal_moves, valid_moves, sizeof(Move) * valid_move_count); 
    pos->legal_move_count = valid_move_count;
}

