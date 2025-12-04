
#include "opponent/opponent.h"

Move* pick_move(Move* legal_moves)
{
    // int c = get_legal_move_count(legal_moves);
    // TODO pick a move
    return &legal_moves[0];
}

int opponent_move(Position* position)
{
    Move* m = pick_move(position->legal_moves);
    Undo undo = {};
    if (handle_move(position, m->startsquare, m->destsquare, &undo))
    {
        return 0;
    }
    return 1;
}
