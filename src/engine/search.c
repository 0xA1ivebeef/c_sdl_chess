
#include "engine/search.h"

static uint64_t nodes = 0;

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

int compare_moves(const void* a, const void* b) 
{
    MoveScore* ma = (MoveScore*)a;
    MoveScore* mb = (MoveScore*)b;
    return mb->score - ma->score;  // descending
}

int is_capture_move(uint64_t bb, Move m)
{
    return ((bb & (1ULL << move_to(m))) > 0);
}

int gives_check(Position* pos, Move m)
{
    Undo undo;
    apply_move(pos, m, &undo);
    int check = is_check(pos, pos->player);  
    undo_move(pos, m, &undo);

    return check;
}

void sort_mvv_lva(Position* pos, LegalMoves* lm)
{
    MoveScore moves_with_scores[LEGAL_MOVES_SIZE];
    for (int i = 0; i < lm->count; ++i)
    {
        // enpassant is not included in this but should be fine
        int score = 0;

        if (is_capture_move(pos->bb[get_bb_index(pos->bb, move_from(lm->moves[i]))], lm->moves[i]))
        {
            int victim_val = PIECE_VALUES[get_bb_index(pos->bb, move_to(lm->moves[i])) % 5];
            int attacker_val = PIECE_VALUES[get_bb_index(pos->bb, move_from(lm->moves[i])) % 5];

            score += 1000 + (victim_val - attacker_val);
        }

        if (gives_check(pos, lm->moves[i]))
            score += 900;

        if (is_promotion_move(lm->moves[i]))
            score += 1000;

        moves_with_scores[i] = (MoveScore) { lm->moves[i], score } ;
    }

    qsort(moves_with_scores, lm->count, sizeof(struct MoveScore), compare_moves);

    for (int i = 0; i < lm->count; ++i)
        lm->moves[i] = moves_with_scores[i].move;
}

int alphabeta(Position* pos, int depth, int alpha, int beta)
{
    nodes++;
    if (depth == 0)
        return evaluate(pos);

    LegalMoves lm;
    generate_legal_moves(pos, &lm);
    filter_moves(pos, &lm);

    if (lm.count == 0)
    {
        if (is_check(pos, pos->player))
            return -MATE;
        else 
            return 0;
    }

    sort_mvv_lva(pos, &lm);

    int best = -INF;

    for (int i = 0; i < lm.count; i++)
    {
        Undo undo;
        apply_move(pos, lm.moves[i], &undo);

        int score = -alphabeta(pos, depth - 1, -beta, -alpha);

        undo_move(pos, lm.moves[i], &undo);

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
int search(Position* pos, int depth)
{
    nodes++;
    if (depth == 0)
        return evaluate(pos);

    LegalMoves lm;
    generate_legal_moves(pos, &lm);
    filter_moves(pos, &lm);

    if (lm.count == 0)
    {
        if (is_check(pos, pos->player))
            return -MATE;
        else 
            return 0;
    }

    int best_eval = -INF;

    for (int i = 0; i < lm.count; ++i)
    {
        Undo undo;
        apply_move(pos, lm.moves[i], &undo);

        int eval = -search(pos, depth - 1);
        best_eval = max(eval, best_eval);

        undo_move(pos, lm.moves[i], &undo);
    }

    return best_eval; 
}

Move search_root(Position* pos, int depth)
{
    LegalMoves lm;

    generate_legal_moves(pos, &lm);
    filter_moves(pos, &lm);

    if (lm.count == 0)
        return 0;

    sort_mvv_lva(pos, &lm);

    Move best_move = 0;
    int best_eval = -INF;

    for (int i = 0; i < lm.count; ++i)
    {
        Undo undo;
        apply_move(pos, lm.moves[i], &undo);

        // negamax so this returns a large number if the position 
        // is good for the player who made the move
        int eval = -alphabeta(pos, depth - 1, -INF, INF); 
        if (eval > best_eval)
        {
            best_eval = eval;
            best_move = lm.moves[i];
        }

        undo_move(pos, lm.moves[i], &undo);
    }

    return best_move;
}

void search_test(Position* pos)
{
    printf("classical search\n");
    double start = get_time_seconds();
    search(pos, 5);
    double end = get_time_seconds();
    printf("EXECUTION TIME: %f seconds\n", end-start);
    printf("NODES SEARCHED %zu\n", nodes);

    nodes = 0;
    printf("alpha beta search\n");
    start = get_time_seconds();
    alphabeta(pos, 5, -INF, INF);
    end = get_time_seconds();
    printf("EXECUTION TIME: %f seconds\n", end-start);
    printf("NODES SEARCHED %zu\n", nodes);
}

