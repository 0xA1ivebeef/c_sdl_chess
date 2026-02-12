
#include "ui/console_logger.h"

void print_castle_rights(int castle_rights) 
{
    // Print 4 bits: BLACK_QS, BLACK_KS, WHITE_QS, WHITE_KS
    for(int i = 3; i >= 0; --i) 
	{
        putchar( (castle_rights & (1 << i)) ? '1' : '0' );
    }
    putchar('\n');
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

    printf("king sq: BLACK: %d, WHITE: %d\n", pos->king_sq[BLACK], pos->king_sq[WHITE]);
    printf("\n --------------------------------------------\n\n");
}

void get_sq_string(int sq, char* buffer)
{
    int col = sq % 8;
    int row = sq / 8;
    buffer[0] = (char)(col + 97);
    buffer[1] = (char)(8 - row + '0');
    buffer[2] = '\0';
}

void translate_move(Move* m)
{
    char start_str[3];
    char dest_str[3];

    get_sq_string(m->start, start_str);
    get_sq_string(m->dest, dest_str);

    printf("%s, %s\n", start_str, dest_str);
}

void log_bitboard(uint64_t* n)
{
	for (int i = 0; i < 64; ++i)
	{
		if (*n & (1ULL << i))
			printf("# ");
		else
			printf(". ");

		if ((i + 1) % 8 == 0)
			printf("\n");
	}
    printf("\n\n");
}

void log_bitboards(uint64_t* bitboards, uint64_t* occupancy_bitboards)
{
    printf("bitboards:\n");
    for(int i = 0; i < 12; ++i)
        log_bitboard(&bitboards[i]);

    if(!occupancy_bitboards)
        return;

    printf("occupancy_bitboards:\n");
    for(int i = 0; i < 3; ++i)
        log_bitboard(&occupancy_bitboards[i]);
}

void log_legal_moves(Move* legal_moves)
{
	for(int i = 0; i < LEGAL_MOVES_SIZE; ++i)
    {
        if(legal_moves[i].start == legal_moves[i].dest)
            return;

        printf("%d: ", i+1);
        translate_move(&legal_moves[i]);
    }
}

