
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

void sort_moves(Position* pos, LegalMoves* lm, Move best_move)
{
    MoveScore moves_with_scores[LEGAL_MOVES_SIZE];
    for (int i = 0; i < lm->count; ++i)
    {
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

        if (lm->moves[i] == best_move)
            score += 1000*1000;

        moves_with_scores[i] = (MoveScore) { lm->moves[i], score } ;
    }

    qsort(moves_with_scores, lm->count, sizeof(struct MoveScore), compare_moves);

    for (int i = 0; i < lm->count; ++i)
        lm->moves[i] = moves_with_scores[i].move;
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
            return alpha;
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

    // TODO play TT move first
    sort_moves(pos, &lm, entry->best_move);

    int best = -INF;
    Move best_move = 0;
    for (int i = 0; i < lm.count; i++)
    {
        Undo undo;
        apply_move(pos, lm.moves[i], &undo);

        int score = -alphabeta(pos, depth - 1, -beta, -alpha);

        undo_move(pos, lm.moves[i], &undo);

        if (score > best)
        {
            best = score;
            best_move = lm.moves[i];
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

int alphanott(Position* pos, int depth, int alpha, int beta)
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
            return -MATE + depth; // faster mates should be prio
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
        {
            best = score;
        }

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

    printf("tt hits %d\n", tt_hits);

    return best_move;
}

void search_test(Position* pos)
{
    printf("classical search\n");
    double start = get_time_seconds();
    search(pos, 4);
    double end = get_time_seconds();
    printf("EXECUTION TIME: %f seconds\n", end-start);
    printf("NODES SEARCHED %zu\n", nodes);

    nodes = 0;
    printf("alpha beta search\n");
    start = get_time_seconds();
    alphabeta(pos, 4, -INF, INF);
    end = get_time_seconds();
    printf("EXECUTION TIME: %f seconds\n", end-start);
    printf("NODES SEARCHED %zu\n", nodes);
}

