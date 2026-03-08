
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

    int move_count = pos->legal_move_count;
    Move moves[LEGAL_MOVES_SIZE];
    memcpy(moves, pos->legal_moves, sizeof(Move) * move_count);

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

typedef struct
{
    int nodes;
    Move move;
} move_node;

void perft_divide(Position* pos, int depth, move_node* move_nodes)
{
    generate_legal_moves(pos);
    filter_moves(pos);

    int move_count = pos->legal_move_count;
    Move moves[LEGAL_MOVES_SIZE];
    memcpy(moves, pos->legal_moves, sizeof(Move) * move_count);

    int total = 0;
    for (int i = 0; i < move_count; ++i)
    {
        Move m = moves[i];
        Undo undo = {0};

        Position backup = *pos;
        save_state(pos, &m, &undo);
        apply_move(pos, &m);

        move_nodes[i] = (move_node) { get_nodes(pos, depth - 1), moves[i] };

        undo_move(pos, &m, &undo);

        if (memcmp(&backup, pos, sizeof(Position)))
        {
            fprintf(stderr, "MEMCMP RETURNED 1\n");
            printf("move: %s, %s\n", square_to_notation(m.start), square_to_notation(m.dest));
            log_position_diff(pos, &backup);
            exit(-1);
        }

        total += move_nodes[i].nodes;
    }
    printf("total: %d\n", total);
}

void perft(Position* pos, int depth)
{
    move_node move_nodes[255] = {0};
    perft_divide(pos, depth, move_nodes);

    int i = 0;
    while(move_nodes[i].nodes)
    {
        printf("%d: %s, %s | %d NODES\n", i+1, square_to_notation(move_nodes[i].move.start), square_to_notation(move_nodes[i].move.dest), move_nodes[i].nodes);
        ++i;
    }
}

