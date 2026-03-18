
#include "engine/search.h"

static int tt_hits = 0;
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

int gives_check(Position* pos, Move m)
{
    Undo undo;
    apply_move(pos, m, &undo);
    int check = is_check(pos, pos->player);  
    undo_move(pos, m, &undo);

    return check;
}

/*
    Typical strong engine ordering:

    TT move / best move
    Good captures (MVV-LVA)
    Promotions
    Killer moves
    History heuristic moves
    Checks (optional / often implicit)
    Bad captures (SEE < 0)
*/

#define SCORE_TT_MOVE      1000000
#define SCORE_GOOD_CAPTURE  100000
#define SCORE_PROMOTION      90000
#define SCORE_KILLER         80000
#define SCORE_HISTORY         1000

void score_moves(Position* pos, LegalMoves* lm, int* scores, Move best_move)
{
    for (int i = 0; i < lm->count; ++i)
    {
        Move m = lm->moves[i];
        scores[i] = 0;

        if (m == best_move)
        {
            scores[i] = SCORE_TT_MOVE;
            continue;
        }

        // capture
        if (pos->occ[!pos->player] & (1ULL << move_to(m))) 
        {
            // TODO cache a pieces bitboard dont use get_bb_index
            int victim = PIECE_VALUES[get_bb_index(pos->bb, move_to(lm->moves[i])) % 5];
            int attacker = PIECE_VALUES[get_bb_index(pos->bb, move_from(lm->moves[i])) % 5];

            scores[i] += SCORE_GOOD_CAPTURE + (victim*10 - attacker);
            continue;
        }


        if (is_promotion_move(m))
        {
            scores[i] = SCORE_PROMOTION;
            continue;
        }

        // TODO killer moves, history
        
        if (gives_check(pos, m))
            scores[i] = 100;
    }
}

Move pick_next_move(LegalMoves* lm, int* scores, int start)
{
    int best = start;

    for (int i = start + 1; i < lm->count; i++)
        if (scores[i] > scores[best])
            best = i;

    if (best != start)
    {
        // swap moves
        Move tmp_move = lm->moves[start];
        lm->moves[start] = lm->moves[best];
        lm->moves[best] = tmp_move;

        // swap scores
        int tmp_score = scores[start];
        scores[start] = scores[best];
        scores[best] = tmp_score;
    }

    return lm->moves[start];
}

// alpha = best value MAX can gurantee so far (lower bound of what MAX can achieve)
// beta  = best value MIN can gurantee so far (upper bound of what MIN can force)
//
// if a >= b stop exploring because MAX already has a better option elsewhere
// if its a MAX node and MIN has a better option if its a MIN node
//
// no possible value subtree can produce that would matter to the parent
int alphabeta(Position* pos, int depth, int alpha, int beta)
{
    nodes++;
    if (depth == 0)
        return evaluate(pos);

    int orig_alpha = alpha;
    int orig_beta = beta;

    TTEntry* entry = TT_lookup(pos->hash);
    if (entry && entry->depth >= depth)
    {
        tt_hits++;
        if (entry->flag == TT_EXACT)
            return entry->eval;
        
        if (entry->flag == TT_LOWERBOUND)
            alpha = max(alpha, entry->eval);

        if (entry->flag == TT_UPPERBOUND)
            beta = min(beta, entry->eval);

        if (alpha >= beta) // algorithm condition (prune)
            return entry->eval;
    }

    LegalMoves lm;
    generate_legal_moves(pos, &lm);
    filter_moves(pos, &lm);

    if (lm.count == 0)
    {
        if (is_check(pos, pos->player))
            return -MATE + depth; // faster mates should be prio
        else 
            return 0;
    }

    int scores[LEGAL_MOVES_SIZE] = {0};
    Move tmp = 0;
    if (entry)
        tmp = entry->best_move;
    score_moves(pos, &lm, scores, tmp);

    int best = -INF;
    Move best_move = 0;
    for (int i = 0; i < lm.count; i++)
    {
        Undo undo;
        Move this_move = pick_next_move(&lm, scores, i);
        apply_move(pos, this_move, &undo);

        int score = -alphabeta(pos, depth - 1, -beta, -alpha);

        undo_move(pos, this_move, &undo);

        if (score > best)
        {
            best = score;
            best_move = this_move;
        }

        if (best > alpha)
            alpha = best;

        if (alpha >= beta)
            break;  // prune remaining moves
    }

    TTEntry new_entry = { pos->hash, best_move, depth, best, 0 };
    if (best <= orig_alpha)
        new_entry.flag = TT_UPPERBOUND;
    else if (best >= orig_beta)
        new_entry.flag = TT_LOWERBOUND;
    else
        new_entry.flag = TT_EXACT;

    TT_store(&new_entry);

    return best;
}

Move search_root(Position* pos, int depth)
{
    LegalMoves lm;
    generate_legal_moves(pos, &lm);
    filter_moves(pos, &lm);

    if (lm.count == 0)
        return 0;

    int scores[LEGAL_MOVES_SIZE] = {0};
    score_moves(pos, &lm, scores, 0);

    Move best_move = 0;
    int best_eval = -INF;

    for (int i = 0; i < lm.count; ++i)
    {
        Undo undo;
        Move this_move = pick_next_move(&lm, scores, i);
        apply_move(pos, this_move, &undo);

        // negamax so this returns a large number if the position 
        // is good for the player who made the move
        int eval = -alphabeta(pos, depth - 1, -INF, INF); 
        if (eval > best_eval)
        {
            best_eval = eval;
            best_move = this_move;
        }

        undo_move(pos, this_move, &undo);
    }

    printf("tt hits %d\n", tt_hits);

    return best_move;
}

void search_test(Position* pos)
{
    nodes = 0;
    printf("alpha beta search\n");
    double start = get_time_seconds();
    alphabeta(pos, 5, -INF, INF);
    double end = get_time_seconds();
    printf("EXECUTION TIME: %f seconds\n", end-start);
    printf("NODES SEARCHED %zu\n", nodes);
    printf("TT Hits: %d\n", tt_hits);
}

