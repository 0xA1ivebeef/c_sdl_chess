
#include "ui/console_logger.h"


void print_uint16_binary(uint16_t n)
{
    for (int i = 15; i >= 0; i--) 
    {
        printf("%d", (n >> i) & 1);

        if (i == 12 || i == 6) 
        {
            printf(" ");
        }
    }
}

void log_move(Move m)
{
    printf("promotion: %d\n", (m >> 12));
    printf("%s %s %d %d\n", square_to_notation(move_from(m)), square_to_notation(move_to(m)), move_from(m), move_to(m));

    printf("int move: %d\n", m);
    printf("bin move: \n");
    print_uint16_binary(m);
    printf("\n");
}

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

    for (int i = 0; i < 3; i++)
    {
        if (a->occ[i] != b->occ[i])
        {
            printf("%d occ mutated logging a then b\n", i);
            log_bitboard(&a->occ[i]);
            log_bitboard(&b->occ[i]);
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

void print_castle_rights(int castle_rights) 
{
    // Print 4 bits: BLACK_QS, BLACK_KS, WHITE_QS, WHITE_KS
    for(int i = 3; i >= 0; --i) 
	{
        putchar( (castle_rights & (1 << i)) ? '1' : '0' );
    }
    putchar('\n');
}

void log_bitboard(uint64_t* n)
{
    printf("\n");
    printf("8 ");
	for (int i = 0; i < 64; ++i)
	{
		if (*n & (1ULL << i))
			printf("# ");
		else
			printf(". ");

		if ((i + 1) % 8 == 0 && i != 63)
			printf("\n%d ", 7 - i/8); // i = 7, 15, 23, 31 .. -> 7, 6, 5 .. 
	}
    printf("\n  a b c d e f g h\n");
    printf("\n\n");
}

void log_occ(uint64_t* occ)
{
    printf("logging occupancy bitboards\n");
    for(int i = 0; i < 3; ++i)
        log_bitboard(&occ[i]);
}

void log_bitboards(uint64_t* bitboards, uint64_t* occ)
{
    if (!bitboards)
    {
        log_occ(occ);
        return;
    }

    printf("bitboards:\n");
    for(int i = 0; i < 12; ++i)
        log_bitboard(&bitboards[i]);

    if (!occ)
        return;

    printf("occupancy_bitboards:\n");
    for(int i = 0; i < 3; ++i)
        log_bitboard(&occ[i]);
}

void log_legal_moves(LegalMoves* lm)
{
	for(int i = 0; i < lm->count; ++i)
    {
        printf("%d: ", i+1);
        printf("%s, %s\n", square_to_notation(move_from(lm->moves[i])), 
                           square_to_notation(move_to(lm->moves[i])));
    }
}

void log_gamestate(Position* pos)
{
    printf("\n --------------------------------------------\n\n");
    printf("logging GAMESTATE: \n");   
    
    printf("current player: %s\n", pos->player ? "WHITE" : "BLACK");
    
    printf("castle_rights: ");
    print_castle_rights(pos->castle_rights);
    printf("\n");

    printf("enpassant sq: %d = %s\n", pos->enpassant, square_to_notation(pos->enpassant));

    printf("halfmove clock: %d\n", pos->halfmove);
    printf("fullmove number: %d\n", pos->fullmove);

    printf("zobrist hash: %zu\n", pos->zobrist_hash);
    printf("hexadecimal: 0x%" PRIX64 "\n", pos->zobrist_hash);

    printf("\n --------------------------------------------\n\n");
}

