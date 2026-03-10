
#include "opponent/opponent.h"

#define INF 1000000
#define MATE 3200

const int PAWN_VAL = 100;
const int KNIGHT_VAL = 300;
const int BISHOP_VAL = 300;
const int ROOK_VAL = 500;
const int QUEEN_VAL = 900;

const int piece_values[5] = 
{
    PAWN_VAL,
    KNIGHT_VAL,
    BISHOP_VAL,
    ROOK_VAL,
    QUEEN_VAL
};

int count_material(uint64_t* bb, uint8_t player)
{
    int res = 0;
    int base = player*6;
    for (int i = 0; i < 5; ++i)
        res += __builtin_popcountll(bb[base + i]) * piece_values[i];

    return res;
}

int evaluate(Position* pos)
{
    int black_material = count_material(pos->bb, BLACK);
    int white_material = count_material(pos->bb, WHITE);

    return white_material - black_material;
}

int search(Position* pos, int depth)
{
    if (depth == 0)
        return evaluate(pos);

    generate_legal_moves(pos);
    filter_moves(pos);

    uint8_t move_count = pos->legal_move_count;
    if (move_count == 0)
    {
        if (is_check(pos))
            return -MATE; // checkmate
        else
            return 0; // stalemate
    }

    int best_eval = -INF;

    Move moves[LEGAL_MOVES_SIZE];
    memcpy(moves, pos->legal_moves, sizeof(Move) * move_count);

    for (int i = 0; i < move_count; ++i)
    {
        Undo undo = {0};
        Move m = moves[i];

        // Position backup = *pos;
        save_state(pos, &m, &undo);
        apply_move(pos, &m);

        int eval = -search(pos, depth - 1); // negamax so you always maximize
        best_eval = max(eval, best_eval); // if its black to move it should be min

        undo_move(pos, &m, &undo);
    }

    return best_eval;
}

Move* search_root(Position* pos, int depth)
{
    Move* best_move = NULL;
    int best_eval = -INF;

    uint8_t move_count = pos->legal_move_count; // needed for the loop

    Move moves[LEGAL_MOVES_SIZE];
    memcpy(moves, pos->legal_moves, sizeof(Move) * move_count);

    for (int i = 0; i < move_count; i++)
    {
        Move m = moves[i];
        Undo undo = {0};

        save_state(pos, &m, &undo);
        apply_move(pos, &m);

        int eval = -search(pos, depth - 1);
        printf("%s%s %d\n", square_to_notation(m.start), square_to_notation(m.dest), eval);

        undo_move(pos, &m, &undo);

        if (eval > best_eval)
        {
            best_eval = eval;
            best_move = &moves[i];
        }
    }

    return best_move;
}

int opponent_move(Position* pos)
{
    Move* m = search_root(pos, 3);
    if (!m)
        return -1;
        
    printf("picked move: %s%s\n", square_to_notation(m->start), square_to_notation(m->dest));

    return handle_move(pos, m); 
}

