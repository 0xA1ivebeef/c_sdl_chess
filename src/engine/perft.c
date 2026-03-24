
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

        make_move(pos, lm.moves[i], &undo);
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
        Move m = lm.moves[i];
        Undo undo;
        make_move(pos, m, &undo);
        move_nodes[i] = (MoveNode) { get_nodes(pos, depth - 1), m };
        // printf("perft divide finished move %s%s\n", square_to_notation(move_from(m)), square_to_notation(move_to(m)));
        undo_move(pos, m, &undo);

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
        // log_move(move_nodes[i].move);
        // printf("%d\n", move_nodes[i].nodes);
        ++i;
    }
    return total;
}

int perft_suite(Position* pos)
{
    int res = 0;
    const int depth = 5; // for now
    for (int i = 0; i < FEN_STRING_COUNT; ++i)
    {
        memset(pos, 0, sizeof(Position));

        int expected = FEN_STRING_MOVE_COUNT_DEPTH_5[i];

        // init
        load_fen_string(pos, i);
        generate_occ(pos);
        generate_piece_on_sq(pos);
        pos->hash = get_zobrist_hash(pos);

        double start = get_time_seconds();
        int res = perft(pos, depth);
        double end = get_time_seconds();
        if (res != expected)
        {
            printf("test %d failed, expected: %d, actual: %d\n", i+1, expected, res);
            res = -1;
        }
        else
            printf("test %d successfull\n", i+1);
        printf("EXECUTION TIME: %f\n", end-start);
    }
    return res;
}

int perft_captures(Position* pos)
{
    LegalMoves lm, cm;
    generate_legal_moves(pos, &lm);
    generate_captures(pos, &cm);

    printf("legal moves:\n");
    log_legal_moves(&lm);

    printf("captures:\n");
    log_legal_moves(&cm);

    int local_count = cm.count;

    // every capture move must be in legal moves
    for (int i = 0; i < cm.count; ++i)
    {
        int found = 0;
        Move capture = cm.moves[i];
        for (int j = 0; j < lm.count; ++j)
        {
            if (capture == lm.moves[j])
                found = 1;
        }

        if (!found)
        {
            printf("error capture move is not in legal moves\n");
            log_move(capture);
            return -1;
        }
    }

    uint64_t attacks = get_attack_bb(pos, pos->player);
    while (attacks) 
    {
        // there must be at least one move in captures that has this sq as dest 
        // all captures must be resolvable through attack bitboard
        int dest = __builtin_ctzll(attacks);

        // find moves in cm   
        for (int i = 0; i < cm.count; ++i)
        {
            if (move_to(cm.moves[i]) == dest)
                local_count--;
        }

        attacks &= attacks - 1;
    }

    // every dest square of captures must have occ[!player] bit set
    for (int i = 0; i < cm.count; ++i)
    {
        if (((1ULL << move_to(cm.moves[i]) & pos->occ[!pos->player])) == 0)
        {
            printf("move is not a capture\n");
            log_move(cm.moves[i]);
            return -1;
        }
    }

    // local_count must be zero
    if (local_count)
    {
        printf("could not resolve all captures to attack bitboard\n");
        return -1;
    }

    return 0;
}

int perft_captures_suite(Position* pos)
{
    // perft_captures on different positions maybe also along a search tree
    int res = 0;

    for (int i = 0; i < FEN_STRING_COUNT; ++i)
    { 
        memset(pos, 0, sizeof(Position));

        // load ith position
        load_fen_string(pos, i);
        generate_occ(pos);
        generate_piece_on_sq(pos);
        pos->hash = get_zobrist_hash(pos);

        if (perft_captures(pos) != 0)
        {
            printf("test %d: perft_captures failed\n", i+1);
            res = -1;
        }
        else
            printf("test %d: success\n", i+1);
    }
    return res;
}
