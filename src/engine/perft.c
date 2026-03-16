
#include "engine/perft.h"

int get_nodes(Position* pos, int depth) 
{
    if (depth == 0)
        return 1;

    LegalMoves lm;
    generate_legal_moves(pos, &lm);
    filter_moves(pos, &lm);

    if (depth == 1)
        return lm.count;

    int nodes = 0;

    for (int i = 0; i < lm.count; ++i)
    {
        Undo undo;

        apply_move(pos, lm.moves[i], &undo);
        nodes += get_nodes(pos, depth - 1);
        undo_move(pos, lm.moves[i], &undo);
    }

    return nodes;
}

int perft_divide(Position* pos, int depth, MoveNode* move_nodes)
{
    LegalMoves lm;
    generate_legal_moves(pos, &lm);
    filter_moves(pos, &lm);

    int total = 0;
    for (int i = 0; i < lm.count; ++i)
    {
        Undo undo;

        apply_move(pos, lm.moves[i], &undo);

        move_nodes[i] = (MoveNode) { get_nodes(pos, depth - 1), lm.moves[i] };
        undo_move(pos, lm.moves[i], &undo);

        total += move_nodes[i].nodes;
    }

    printf("total: %d\n", total);
    return total;
}

int perft(Position* pos, int depth)
{
    MoveNode move_nodes[LEGAL_MOVES_SIZE] = {0};
    int total = perft_divide(pos, depth, move_nodes);

    int i = 0;
    while(move_nodes[i].nodes)
    {
        printf("%s%s: %d\n", square_to_notation(move_from(move_nodes[i].move)), square_to_notation(move_to(move_nodes[i].move)), move_nodes[i].nodes);
        ++i;
    }
    return total;
}

void full_perft_test(Position* pos)
{
    const int depth = 5; // for now
    for (int i = 0; i < FEN_STRING_COUNT; ++i)
    {
        LegalMoves lm;
        memset(pos, 0, sizeof(Position));

        int expected = FEN_STRING_MOVE_COUNT_DEPTH_5[i];

        // init
        load_fen_string(pos, i);
        generate_occ(pos);
        generate_legal_moves(pos, &lm); 
        pos->hash = get_zobrist_hash(pos);

        double start = get_time_seconds();
        int res = perft(pos, depth);
        double end = get_time_seconds();
        if (res != expected)
            printf("test %d failed, expected: %d, actual: %d\n", i+1, expected, res);
        else
            printf("test %d successfull\n", i+1);
        printf("EXECUTION TIME: %f\n", end-start);
    }
}

