
#include "ui/console_logger.h"

void get_square_string(int square, char* buffer)
{
    int col = square % 8;
    int row = square / 8;
    buffer[0] = (char)(col + 97);
    buffer[1] = (char)(8 - row + '0');
    buffer[2] = '\0';
}

void translate_move(Move* m)
{
    char startsquare_str[3];
    char destsquare_str[3];

    get_square_string(m->startsquare, startsquare_str);
    get_square_string(m->destsquare, destsquare_str);

    printf("%s, %s\n", startsquare_str, destsquare_str);
}

void log_bitboard(uint64_t* n)
{
	for (int i = 0; i < 64; ++i)
	{
		if (*n & (1ULL << i))
			printf("1");
		else
			printf("0");

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
        if(legal_moves[i].startsquare == legal_moves[i].destsquare)
            return;

        printf("%d: ", i+1);
        translate_move(&legal_moves[i]);
    }
}

