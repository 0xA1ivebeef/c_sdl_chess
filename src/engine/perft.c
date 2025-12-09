
#include "engine/perft.h"

int get_nodes(Position* position, int depth) 
{
    if (depth == 0)
        return 1;

    generate_legal_moves(position);
    filter_moves(position);

    if (depth == 1)
        return position->legal_move_count;

    int nodes = 0;

    // Snapshot move list
    int move_count = position->legal_move_count;
    Move moves[256]; // or your max move list size

    for (int i = 0; i < move_count; ++i)
        moves[i] = position->legal_moves[i];

    for (int i = 0; i < move_count; ++i)
    {
        Undo undo = {0};

        Move m = moves[i];

        apply_move(position, &m, &undo);

        nodes += get_nodes(position, depth - 1);

        undo_move(position, &m, &undo);
    }

    return nodes;
}

void perft_divide(Position* pos, int depth)
{
    generate_legal_moves(pos);
    filter_moves(pos);

    int move_count = pos->legal_move_count;
    Move moves[256];

    for (int i = 0; i < move_count; ++i)
        moves[i] = pos->legal_moves[i];

    int total = 0;

    for (int i = 0; i < move_count; ++i)
    {
        Move m = moves[i];
        Undo u = {0};

        apply_move(pos, &m, &u);

        int nodes = get_nodes(pos, depth - 1);

        undo_move(pos, &m, &u);

        printf("%s%s: %d\n",
            square_to_notation(m.startsquare),
            square_to_notation(m.destsquare),
            nodes);

        total += nodes;
    }

    printf("total: %d\n", total);
}

