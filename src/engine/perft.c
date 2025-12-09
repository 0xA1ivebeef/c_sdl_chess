
#include "engine/perft.h"

int get_nodes(Position* position, int depth) 
{
    if (depth == 1) 
        return position->legal_move_count;

    if (position->legal_move_count == 0)
        return 0;

    int nodes = 0;

    for (int i = 0; i < position->legal_move_count; ++i) 
    {
        Undo undo = {0};

        // apply move and handle special moves 
        apply_move(position, &position->legal_moves[i], &undo);

        nodes += get_nodes(position, depth - 1);

        undo_move(position, &position->legal_moves[i], &undo);
    }
    return nodes;
}

void perft_divide(Position* pos, int depth)
{
    if (depth < 2) return;

    int total = 0;
    for (int i = 0; i < pos->legal_move_count; ++i)
    {
        Move m = pos->legal_moves[i];
        Undo u = {0};

        apply_move(pos, &m, &u);
        int nodes = get_nodes(pos, depth - 1);
        undo_move(pos, &m, &u);

        printf("%s%s: %d\n", square_to_notation(m.startsquare), square_to_notation(m.destsquare), nodes);
        total += nodes;
    }

    printf("total: %d\n", total);
}

