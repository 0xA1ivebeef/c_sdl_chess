
#include "engine/check.h"

int is_check(Position* pos, uint8_t player)
{
    int king_sq = get_king_sq(pos, player);
    if (king_sq == INVALID_SQUARE)
    {
        fprintf(stderr, "is_check unknown error, king not found\n");
        exit(EXIT_FAILURE);
    }

    uint64_t atk = get_attack_bb(pos, !player);

    return ((atk & (1ULL << king_sq)) != 0);
}

void filter_moves(Position* pos, LegalMoves* lm)
{
    int write_ptr = 0;
    for (int i = 0; i < lm->count; i++)
    {
        Undo undo;  
        Move m = lm->moves[i];
        apply_move(pos, m, &undo);  

        if (!is_check(pos, !pos->player))
            lm->moves[write_ptr++] = m;

        undo_move(pos, m, &undo); 
    }
    lm->count = write_ptr;
}

