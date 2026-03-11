
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

int is_capture_move(uint64_t bb, Move* m)
{
    return ((bb & (1ULL << m->dest)) > 0);
}

int gives_check(Position* pos, Move* m)
{
    Undo undo;
    save_state(pos, m, &undo); 
    apply_move(pos, m);
    int check = is_check(pos);  
    undo_move(pos, m, &undo);

    return check;
}

int is_promotion_move(Move* m)
{
    return (m->flags >= KNIGHT_PROMOTION && m->flags <= QUEEN_PROMOTION);
}

void sort_mvv_lva(Position* pos)
{
    MoveScore moves_with_scores[LEGAL_MOVES_SIZE];
    for (int i = 0; i < pos->legal_move_count; ++i)
    {
        // enpassant is not included in this but should be fine
        int score = 0;

        if (is_capture_move(pos->bb[get_bb_index(pos->bb, pos->legal_moves[i].start)], &pos->legal_moves[i]))
        {
            int victim_val = PIECE_VALUES[get_bb_index(pos->bb, pos->legal_moves[i].dest) % 5];
            int attacker_val = PIECE_VALUES[get_bb_index(pos->bb, pos->legal_moves[i].start) % 5];

            score += 1000 + (victim_val - attacker_val);
        }

        if (gives_check(pos, &pos->legal_moves[i]))
            score += 900;

        if (is_promotion_move(&pos->legal_moves[i]))
            score += 1000;

        moves_with_scores[i] = (MoveScore) { pos->legal_moves[i], score } ;
    }

    qsort(moves_with_scores, pos->legal_move_count, sizeof(struct MoveScore), compare_moves);

    for (int i = 0; i < pos->legal_move_count; ++i)
        pos->legal_moves[i] = moves_with_scores[i].move;
}

int square_under_attack(uint8_t sq, uint64_t attack_bb)
{
    return ((attack_bb & (1ULL << sq)) != 0);
}

int is_check(Position* pos)
{
    int king_sq = get_king_sq(pos, pos->player);
    uint64_t atk = get_attack_bb(pos, !pos->player);

    return ((atk & (1ULL << king_sq)) != 0);
}

void filter_moves(Position* pos)
{
    Move valid_moves[LEGAL_MOVES_SIZE] = {0};
    int valid_move_count = 0;

    for (int i = 0; i < pos->legal_move_count; i++)
    {
        Undo undo = {0};  
        Move m = pos->legal_moves[i];

        save_state(pos, &m, &undo);
        
        Position backup = *pos;

        apply_move(pos, &m);  

        uint8_t king_sq = get_king_sq(pos, !pos->player);
        uint64_t attack_bb = get_attack_bb(pos, pos->player);

        /* printf("filter moves: king square: %d\n", king_sq);
        printf("attack bitboard\n");
        log_bitboard(&attack_bb);
        */

        if (!square_under_attack(king_sq, attack_bb))
        {
            // printf("king is not under attack, adding move %d, %d flag %d\n", m.start, m.dest, m.flags);
            valid_moves[valid_move_count++] = m;
        }

        undo_move(pos, &m, &undo); // restore position

        if (memcmp(&backup, pos, sizeof(Position)))
        {
            fprintf(stderr, "MEMCMP RETURNED 1\n");
            printf("move: %s, %s\n", square_to_notation(m.start), square_to_notation(m.dest));
            log_position_diff(pos, &backup);
            exit(-1);
        }
    }

    memcpy(pos->legal_moves, valid_moves, sizeof(Move) * valid_move_count); 
    pos->legal_move_count = valid_move_count;

    sort_mvv_lva(pos);
}

