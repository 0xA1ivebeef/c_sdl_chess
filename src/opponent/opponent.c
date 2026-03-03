
#include "opponent/opponent.h"

Move* pick_move(Move* legal_moves)
{
    // int c = get_legal_move_count(legal_moves);
    // TODO pick a move
    return &legal_moves[0];
}

int opponent_move(Position* pos)
{
    Move* m = pick_move(pos->legal_moves);
    // if (handle_move(pos, m->start, m->dest))
    return 0;
}
