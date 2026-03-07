
#include "engine/check.h"

void log_position_diff(Position* a, Position* b)
{
    for (int i = 0; i < 12; i++)
    {
        if (a->bb[i] != b->bb[i])
        {
            printf("%d bitboard mutated logging a then b\n", i);
            log_bitboard(&a->bb[i]);
            log_bitboard(&b->bb[i]);
        }
    }

    if (a->player != b->player)
        printf("player changed: backup = %d, current = %d\n", a->player, b->player);

    if (a->castle_rights != b->castle_rights)
        printf("castle_rights changed: backup = %d, current = %d\n", a->castle_rights, b->castle_rights);

    if (a->enpassant != b->enpassant)
        printf("enpassant changed: backup = %d, current = %d\n", a->enpassant, b->enpassant);

    if (a->halfmove != b->halfmove)
        printf("halfmove changed: backup = %d, current = %d\n", a->halfmove, b->halfmove);

    if (a->fullmove != b->fullmove)
        printf("fullmove changed: backup = %d, current = %d\n", a->fullmove, b->fullmove);
}

int square_under_attack(uint8_t sq, uint64_t attack_bb)
{
    return ((attack_bb & (1ULL << sq)) != 0);
}

int is_check(int king_sq, uint64_t attack_bb)
{
    return ((attack_bb & (1ULL << king_sq)) != 0);
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

        printf("filter moves: king square: %d\n", king_sq);
        printf("attack bitboard\n");
        log_bitboard(&attack_bb);

        if (!square_under_attack(king_sq, attack_bb))
        {
            printf("king is not under attack, adding move %d, %d flag %d\n", m.start, m.dest, m.flags);
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
}

