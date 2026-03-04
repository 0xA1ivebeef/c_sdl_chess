
#include "engine/check.h"

int square_under_attack(uint8_t sq, uint64_t attack_bb)
{
    return ((attack_bb & (1ULL << sq)) != 0);
}

int is_check(int king_sq, uint64_t attack_bb)
{
    return square_under_attack(king_sq, attack_bb);
}

// TODO fix undo move (assert its not mutating pos)
void filter_moves(Position* pos)
{
    Move valid_moves[LEGAL_MOVES_SIZE] = {0};
    int valid_move_count = 0;

    // move out of check and dont move into check 
    // -> apply every move, if king under attack move was illegal
    for (int i = 0; i < pos->legal_move_count; i++)
    {
        Undo undo = {0};  
        Move m = pos->legal_moves[i];

        apply_move(pos, &m, &undo);  

        // !!! apply move switches current player (now its black to move)

        uint8_t king_sq = get_king_sq(pos, !pos->player);
        uint64_t attack_bb = get_attack_bb(pos, pos->player);
        if (!square_under_attack(king_sq, attack_bb))
            valid_moves[valid_move_count++] = m;

        undo_move(pos, &m, &undo); // restore position
    }

    // update position legal moves to valid moves
    memcpy(pos->legal_moves, valid_moves, sizeof(Move) * valid_move_count); 
    pos->legal_move_count = valid_move_count;
}

