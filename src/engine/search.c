
#include "engine/search.h"

static int tt_hits = 0;
static int tt_probes = 0;
static int tt_cutoffs = 0;
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

int insufficient_material(Position* pos)
{
    if (has_pawn(pos) || has_rook(pos) || has_queen(pos))
        return 0;

    int bn = __builtin_popcountll(pos->bb[BLACK_KNIGHT]);
    int wn = __builtin_popcountll(pos->bb[WHITE_KNIGHT]);
    int bb = __builtin_popcountll(pos->bb[BLACK_BISHOP]);
    int wb = __builtin_popcountll(pos->bb[WHITE_BISHOP]);
        
    int black_minors = bn + bb;
    int white_minors = wn + wb;

    // king vs king
    if (black_minors == 0 && white_minors == 0)
        return 1;

    // minor vs king
    if ((white_minors == 1 && black_minors == 0) ||
        (white_minors == 0 && black_minors == 1))
        return 1;

    // two knights vs king
    if ((wn == 2 && black_minors == 0) || 
        (white_minors == 0 && bn == 2))
        return 1;
        
    // opposite colored bishops are not a draw
    if (wb == 1 && bb == 1)
    {
        int bb_sq = __builtin_ctzll(pos->bb[BLACK_BISHOP]);
        int wb_sq = __builtin_ctzll(pos->bb[WHITE_BISHOP]);

        int bb_col = ((rank(bb_sq) + file(bb_sq)) & 1);
        int wb_col = ((rank(wb_sq) + file(wb_sq)) & 1);

        // same color if 1 1 or 0 0 
        if ((bb_col ^ wb_col) == 0)
            return 1;
    }

    return 0;
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

void score_moves(Position* pos, LegalMoves* lm, int* scores, Move tt_move)
{
    for (int i = 0; i < lm->count; ++i)
    {
        Move m = lm->moves[i];
        scores[i] = 0;

        if (m == tt_move)
        {
            scores[i] = SCORE_TT_MOVE;
            continue;
        }

        // capture
        if (pos->occ[!pos->player] & (1ULL << move_to(m))) 
        {
            int victim = PIECE_VALUES[pos->piece_on_sq[move_to(m)] % 5];
            int attacker = PIECE_VALUES[pos->piece_on_sq[move_from(m)] % 5];

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

// only do quiescence search when position is not check
// alpha min eval, maximizing player can ensure
// beta max eval, minimizing player can ensure?
int q_search(Position* pos, int alpha, int beta)
{
    // evaluate current position
    int s_eval = evaluate(pos);
    if (s_eval >= beta)
        return beta; // beta cutoff

    if (alpha < s_eval)
        alpha = s_eval; // alpha improve
        
    // generate only captures
    LegalMoves lm;
    generate_captures(pos, &lm);
    filter_moves(pos, &lm);
    if (lm.count == 0)
        return evaluate(pos);

    for (int i = 0; i < lm.count; i++)
    {
        Undo undo;
        Move m = lm.moves[i];

        /* TODO implement SEE (static exchange evaluation) 
         * to skip losing captures but this is optimization
        if (see(pos, move) < 0)
            continue;
        */

        apply_move(pos, m, &undo);
        int score = -q_search(pos, -beta, -alpha);
        undo_move(pos, m, &undo);

        if (score >= beta)
            return beta; // cutoff

        if (score > alpha)
            alpha = score;
    } 

    // printf("q_search output: %d\n", alpha);
    return alpha; 
}

int alphabeta(Position* pos, int depth, int alpha, int beta)
{
    nodes++;

    if (depth == 0)
        return q_search(pos, alpha, beta);

    if (insufficient_material(pos))
        return 0;

    int orig_alpha = alpha;
    int orig_beta = beta;

    LegalMoves lm;
    generate_legal_moves(pos, &lm);
    filter_moves(pos, &lm);
    if (lm.count == 0)
    {
        if (is_check(pos, pos->player))
            return -MATE - depth; // faster mates should be prio
        return 0;
    }

    TTEntry* entry = TT_lookup(pos->hash);
    tt_probes++;
    if (entry && entry->depth >= depth)
    {
        tt_hits++;
        if (entry->flag == TT_EXACT)
        {
            tt_cutoffs++;
            return entry->eval;
        }

        if (entry->flag == TT_LOWERBOUND)
            alpha = max(alpha, entry->eval);

        if (entry->flag == TT_UPPERBOUND)
            beta = min(beta, entry->eval);

        if (alpha >= beta) // algorithm condition (prune)
        {
            tt_cutoffs++;
            return entry->eval;
        }
    }

    int scores[LEGAL_MOVES_SIZE] = {0};
    Move tt_move = entry ? entry->best_move : 0;
    score_moves(pos, &lm, scores, tt_move);

    /* 
    for (int i = 0; i < lm.count; ++i)
        printf("Move %s%s Score %d\n", square_to_notation(move_from(lm.moves[i])), square_to_notation(move_to(lm.moves[i])), scores[i]);
    */

    int best = -INF;
    Move best_move = 0;
    for (int i = 0; i < lm.count; i++)
    {
        Undo undo;
        Move m = pick_next_move(&lm, scores, i);
        apply_move(pos, m, &undo);

        int score = -alphabeta(pos, depth - 1, -beta, -alpha);

        undo_move(pos, m, &undo);

        if (score > best)
        {
            best = score;
            best_move = m;
        }

        if (best > alpha)
            alpha = best;

        if (alpha >= beta)
            break;  // prune
    }

    uint8_t flag = TT_EXACT;
    if (best <= orig_alpha) // position is at most this good, maybe worse
        flag = TT_UPPERBOUND;
    else if (best >= orig_beta) // position is at least this good, maybe better
        flag = TT_LOWERBOUND;

    TTEntry new_entry = { pos->hash, best_move, depth, best, flag };
    TT_store(&new_entry);

    return best;
}

Move search_root(Position* pos, int depth)
{
    if (insufficient_material(pos))
        return 0;

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
        Move m = pick_next_move(&lm, scores, i);
        apply_move(pos, m, &undo);

        // negamax so this returns a large number if the position 
        // is good for the player who made the move
        int eval = -alphabeta(pos, depth - 1, -INF, INF); 
        printf("eval: %d\n", eval);
        if (eval > best_eval)
        {
            best_eval = eval;
            best_move = m;
        }

        undo_move(pos, m, &undo);
        printf("finished move %s%s, best eval %d\n", 
                square_to_notation(move_from(m)), 
                square_to_notation(move_to(m)),
                best_eval);
        printf("best move: %s%s\n",
                square_to_notation(move_from(best_move)), 
                square_to_notation(move_to(best_move)));
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
    printf("TT Probes: %d\n", tt_probes);
    printf("TT HIT RATE: %f\n", (float)tt_probes / (float)tt_hits);
    printf("TT CUTOFFS: %d\n", tt_cutoffs); 
    printf("TT cuttoff rate: %f\n", (float)tt_cutoffs / (float)tt_probes); 
}

