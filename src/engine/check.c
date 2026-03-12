
#include "engine/check.h"

typedef struct MoveScore 
{
    Move move;
    int score;
} MoveScore;

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

int is_check(Position* pos, uint8_t player)
{
    int king_sq = get_king_sq(pos, player);
    uint64_t atk = get_attack_bb(pos, !player);

    return ((atk & (1ULL << king_sq)) != 0);
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

void filter_moves(Position* pos, LegalMoves* lm)
{
    Move valid_moves[LEGAL_MOVES_SIZE] = {0};
    int valid_move_count = 0;

    for (int i = 0; i < lm->count; i++)
    {
        Undo undo;  

        apply_move(pos, lm->moves[i], &undo);  

        if (!is_check(pos, !pos->player))
            valid_moves[valid_move_count++] = lm->moves[i];

        undo_move(pos, lm->moves[i], &undo); 
    }

    memcpy(lm->moves, valid_moves, sizeof(Move) * valid_move_count); 
    lm->count = valid_move_count;

    sort_mvv_lva(pos, lm);
}

