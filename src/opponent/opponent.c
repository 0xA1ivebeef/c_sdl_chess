
#include "opponent/opponent.h"

#define INF 1000000
#define MATE 3200

static int get_opening_move = 1;

int count_material(uint64_t* bb, uint8_t player)
{
    int res = 0;
    int base = player*6;
    for (int i = 0; i < 5; ++i)
        res += __builtin_popcountll(bb[base + i]) * PIECE_VALUES[i];

    return res;
}

int evaluate(Position* pos)
{
    int black_material = count_material(pos->bb, BLACK);
    int white_material = count_material(pos->bb, WHITE);

    int eval = white_material - black_material;
    return pos->player == WHITE ? eval : -eval;
}

int alphabeta(Position* pos, LegalMoves* lm, int depth, int alpha, int beta)
{
    if (depth == 0)
        return evaluate(pos);

    generate_legal_moves(pos, lm);
    filter_moves(pos, lm);

    if (lm->count == 0)
    {
        if (is_check(pos, pos->player))
            return -MATE;
        else 
            return 0;
    }

    int best = -INF;

    
    Move legal_moves[LEGAL_MOVES_SIZE];
    memcpy(legal_moves, lm->moves, sizeof(Move) * lm->count);
    
    int move_count = lm->count;

    for (int i = 0; i < move_count; i++)
    {
        Undo undo;
        apply_move(pos, legal_moves[i], &undo);

        int score = -alphabeta(pos, lm, depth - 1, -beta, -alpha);

        undo_move(pos, legal_moves[i], &undo);

        if (score > best)
            best = score;

        if (best > alpha)
            alpha = best;

        if (alpha >= beta)
            break;  // prune remaining moves
    }

    return best;
}

// negamax always assumes returned score
// is from the perspective of the player to move
int search(Position* pos, LegalMoves* lm, int depth)
{
    if (depth == 0)
        return evaluate(pos);

    generate_legal_moves(pos, lm);
    filter_moves(pos, lm);

    if (lm->count == 0)
    {
        if (is_check(pos, pos->player))
            return -MATE;
        else 
            return 0;
    }

    int best_eval = -INF;

    Move legal_moves[LEGAL_MOVES_SIZE];
    memcpy(legal_moves, lm->moves, sizeof(Move) * lm->count);
    
    int move_count = lm->count;

    for (int i = 0; i < move_count; ++i)
    {
        Undo undo;
        apply_move(pos, legal_moves[i], &undo);

        int eval = -search(pos, lm, depth - 1);
        best_eval = max(eval, best_eval);

        undo_move(pos, legal_moves[i], &undo);
    }

    return best_eval; 
}

Move search_root(Position* pos, LegalMoves* lm, int depth)
{
    // black to move
    if (lm->count == 0)
        return 0;

    Move best_move = 0;

    Move legal_moves[LEGAL_MOVES_SIZE];
    memcpy(legal_moves, lm->moves, sizeof(Move) * lm->count);
    
    int move_count = lm->count;

    int best_eval = -INF;

    for (int i = 0; i < move_count; ++i)
    {
        Undo undo;
        apply_move(pos, legal_moves[i], &undo);

        // negamax so this returns a large number if the position 
        // is good for the player who made the move
        int eval = -alphabeta(pos, lm, depth - 1, -INF, INF); 
        if (eval > best_eval)
        {
            best_eval = eval;
            best_move = legal_moves[i];
        }

        undo_move(pos, legal_moves[i], &undo);
    }

    generate_legal_moves(pos, lm);
    filter_moves(pos, lm);

    return best_move;
}

int opponent_move(Position* pos, LegalMoves* lm)
{
    // assuming opening book moves are legal
    if (get_opening_move)
    {
        Move opening_move = get_random_opening_move(pos->zobrist_hash);
        printf("opening book got move:\n");
        log_move(opening_move);
        if (!opening_move)
        {
            printf("opening is finished\n");
            get_opening_move = 0; // dont look for openings anymore
        }
        else
            return handle_move(pos, lm, opening_move);
    }

    Move m = search_root(pos, lm, 5);
    if (!m)
        return -1;
        
    printf("picked move: %s%s\n", square_to_notation(move_from(m)), square_to_notation(move_to(m)));

    return handle_move(pos, lm, m); 
}

