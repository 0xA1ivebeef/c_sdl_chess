
#include "engine/perft.h"

int get_nodes(Position* position, int depth) 
{
    /*
    if (depth == 1) 
        return position->legal_move_count;

    if (position->legal_move_count == 0)
        return 0;

    int nodes = 0;
    for (int i = 0; i < position->legal_move_count; ++i) 
    {
        Undo undo = {0};
        move m = position->legal_moves[i];

        apply_move(position, &m, &undo);
        update_gamestate(position);

        nodes += get_nodes(position, depth - 1);

        undo_move(position, &m, &undo);
    }
    return nodes;
    */
    return 0;
}

