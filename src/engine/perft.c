
#include "engine/perft.h"

int get_nodes(Position* pos, int depth, LegalMoves* lm) 
{
    if (depth == 0)
        return 1;

    generate_legal_moves(pos, lm);
    filter_moves(pos, lm);

    if (depth == 1)
        return lm->count;

    int nodes = 0;

    int move_count = lm->count;
    Move moves[LEGAL_MOVES_SIZE];
    memcpy(moves, lm->moves, sizeof(Move) * move_count);

    for (int i = 0; i < move_count; ++i)
    {
        Move m = moves[i];
        Undo undo;

        apply_move(pos, m, &undo);
        nodes += get_nodes(pos, depth - 1, lm);
        undo_move(pos, m, &undo);
    }

    return nodes;
}

typedef struct
{
    int nodes;
    Move move;
} move_node;

int perft_divide(Position* pos, LegalMoves* lm, int depth, move_node* move_nodes)
{
    generate_legal_moves(pos, lm);
    filter_moves(pos, lm);

    int move_count = lm->count;
    Move moves[LEGAL_MOVES_SIZE];
    memcpy(moves, lm->moves, sizeof(Move) * move_count);

    int total = 0;
    for (int i = 0; i < move_count; ++i)
    {
        Move m = moves[i];
        Undo undo;

        apply_move(pos, m, &undo);
        move_nodes[i] = (move_node) { get_nodes(pos, depth - 1, lm), moves[i] };
        undo_move(pos, m, &undo);

        total += move_nodes[i].nodes;
    }

    printf("total: %d\n", total);
    return total;
}

int perft(Position* pos, LegalMoves* lm, int depth)
{
    move_node move_nodes[LEGAL_MOVES_SIZE] = {0};
    int total = perft_divide(pos, lm, depth, move_nodes);

    int i = 0;
    while(move_nodes[i].nodes)
    {
        printf("%s%s: %d\n", square_to_notation(move_from(move_nodes[i].move)), square_to_notation(move_to(move_nodes[i].move)), move_nodes[i].nodes);
        ++i;
    }
    return total;
}

void full_perft_test(Position* pos, LegalMoves* lm)
{
    const int depth = 5; // for now
        
    for (int i = 0; i < FEN_STRING_COUNT; ++i)
    {
        memset(pos, 0, sizeof(Position));
        memset(lm, 0, sizeof(LegalMoves));

        int expected = FEN_STRING_MOVE_COUNT_DEPTH_5[i];

        // init
        load_fen_string(pos, i);
        update_occ(pos);
        generate_legal_moves(pos, lm); 
        pos->zobrist_hash = get_zobrist_hash(pos);

        int res = perft(pos, lm, depth);
        if (res != expected)
            printf("test %d failed, expected: %d, actual: %d\n", i+1, expected, res);
        else
            printf("test %d successfull\n", i+1);
    }
}

