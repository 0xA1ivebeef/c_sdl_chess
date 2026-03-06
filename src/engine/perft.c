
#include "engine/perft.h"

int get_nodes(Position* pos, int depth) 
{
    if (depth == 0)
        return 1;

    generate_legal_moves(pos);
    filter_moves(pos);

    if (depth == 1)
        return pos->legal_move_count;

    int nodes = 0;

    // Snapshot move list
    int move_count = pos->legal_move_count;
    Move moves[256]; // or your max move list size

    for (int i = 0; i < move_count; ++i)
        moves[i] = pos->legal_moves[i];

    for (int i = 0; i < move_count; ++i)
    {
        Undo undo = {0};

        Move m = moves[i];

        save_state(pos, &m, &undo);
        apply_move(pos, &m);

        nodes += get_nodes(pos, depth - 1);

        undo_move(pos, &m, &undo);
    }

    return nodes;
}

void perft_divide(Position* pos, int depth)
{
    generate_legal_moves(pos);
    filter_moves(pos);

    int move_count = pos->legal_move_count;
    Move moves[255];

    for (int i = 0; i < move_count; ++i)
        moves[i] = pos->legal_moves[i];

    int total = 0;

    for (int i = 0; i < move_count; ++i)
    {
        Move m = moves[i];
        Undo undo = {0};

        save_state(pos, &m, &undo);
        apply_move(pos, &m);

        int nodes = get_nodes(pos, depth - 1);

        undo_move(pos, &m, &undo);

        printf("%s%s: %d\n",
            square_to_notation(m.start),
            square_to_notation(m.dest),
            nodes);

        total += nodes;
    }

    printf("total: %d\n", total);
}

