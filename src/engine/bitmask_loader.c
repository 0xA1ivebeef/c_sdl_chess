
#include "engine/bitmask_loader.h"

uint64_t white_pawn_normal_bitmasks[64] = {0};
uint64_t black_pawn_normal_bitmasks[64] = {0};
uint64_t white_pawn_attack_bitmasks[64] = {0};
uint64_t black_pawn_attack_bitmasks[64] = {0};
uint64_t white_pawn_double_bitmasks[64] = {0};
uint64_t black_pawn_double_bitmasks[64] = {0};
uint64_t knight_bitmasks[64]            = {0};
uint64_t bishop_bitmasks[64]            = {0};
uint64_t rook_bitmasks[64]              = {0};
uint64_t queen_bitmasks[64]             = {0};
uint64_t king_bitmasks[64]              = {0};

const int knight_file_offsets[8] = { 2, 2, 1, -1, -2, -2, -1,  1 };
const int knight_rank_offsets[8] = { -1, 1, 2,  2,  1, -1, -2, -2 };

int is_on_board(int file, int rank)
{
	return (file >= 0 && file < 8 && rank >= 0 && rank < 8);
}

void load_white_pawn_normal_bitmasks(void)
{
    for (int i = 8; i < 56; ++i)
	    white_pawn_normal_bitmasks[i] |= (1ULL << ((rank(i) - 1) * 8 + file(i)));
}

void load_black_pawn_normal_bitmasks(void)
{
    for (int i = 8; i < 56; ++i)
	    black_pawn_normal_bitmasks[i] |= (1ULL << ((rank(i) + 1) * 8 + file(i)));
}

void load_white_pawn_double_bitmasks(void)
{
    for (int i = 48; i < 56; ++i)
	    white_pawn_double_bitmasks[i] |= (1ULL << (i - 16));
}

void load_black_pawn_double_bitmasks(void)
{
    for (int i = 8; i < 16; ++i)
	    black_pawn_double_bitmasks[i] |= (1ULL << (i + 16));
}

void load_white_pawn_attack_bitmasks(void)
{
    for (int i = 8; i < 56; ++i)
    {
	    if (file(i) - 1 >= 0)
		    white_pawn_attack_bitmasks[i] |= (1ULL << ((rank(i) - 1) * 8 + file(i) - 1));

	    if (file(i) + 1 < 8)
		    white_pawn_attack_bitmasks[i] |= (1ULL << ((rank(i) - 1) * 8 + file(i) + 1));
    }
}

void load_black_pawn_attack_bitmasks(void)
{
    for (int i = 8; i < 56; ++i)
    {
	    if (file(i) - 1 >= 0)
		    black_pawn_attack_bitmasks[i] |= (1ULL << ((rank(i) + 1) * 8 + file(i) - 1));

	    if (file(i) + 1 < 8)
		    black_pawn_attack_bitmasks[i] |= (1ULL << ((rank(i) + 1) * 8 + file(i) + 1));
    }
}

void load_knight_bitmasks(void)
{
    int this_file, this_rank;
    for (int i = 0; i < 64; ++i)
    {
	    for (int dir = 0; dir < 8; ++dir)
	    {
		    this_file = file(i) + knight_file_offsets[dir];
		    this_rank = rank(i) + knight_rank_offsets[dir];

		    if (!is_on_board(this_file, this_rank))
			    continue;

		    knight_bitmasks[i] |= (1ULL << (this_rank * 8 + this_file));
	    }
    }
}

void load_bishop_bitmasks(void)
{
    int this_file, this_rank;
    for (int i = 0; i < 64; ++i)
    {
	    for (int dir = 4; dir < 8; ++dir)
	    {
		    for (int j = 1; j < 8; ++j)
		    {
			    this_file = file(i) + j * file_offsets[dir];
			    this_rank = rank(i) + j * rank_offsets[dir];

			    if (!is_on_board(this_file, this_rank))
				    continue;

			    bishop_bitmasks[i] |= (1ULL << (this_rank * 8 + this_file));
		    }
	    }
    }
}

void load_rook_bitmasks(void)
{
    int this_file, this_rank;
    for (int i = 0; i < 64; ++i)
    {
	    this_file = file(i);
	    this_rank = rank(i);
	    for (int j = 0; j < 8; ++j)
	    {
		    rook_bitmasks[i] |= (1ULL << (this_rank * 8 + j));
		    rook_bitmasks[i] |= (1ULL << (this_file * 8 + j));
	    }
	    rook_bitmasks[i] &= ~(1ULL << i);
    }
}

void load_queen_bitmasks(void)
{
    for (int i = 0; i < 64; ++i)
	    queen_bitmasks[i] = bishop_bitmasks[i] | rook_bitmasks[i];
}

void load_king_bitmasks(void)
{
    int this_file, this_rank;
    for (int i = 0; i < 64; ++i)
    {
	    for (int dir = 0; dir < 8; ++dir)
	    {
		    this_file = file(i) + file_offsets[dir];
		    this_rank = rank(i) + rank_offsets[dir];

		    if (!is_on_board(this_file, this_rank))
			    continue;

		    king_bitmasks[i] |= (1ULL << (this_rank * 8 + this_file));
	    }
    }
}

void load_bitmasks(void)
{
    load_white_pawn_normal_bitmasks();
    load_black_pawn_normal_bitmasks();

    load_white_pawn_double_bitmasks();
    load_black_pawn_double_bitmasks();

    load_white_pawn_attack_bitmasks();
    load_black_pawn_attack_bitmasks();

    load_knight_bitmasks();
    load_bishop_bitmasks();
    load_rook_bitmasks();
    load_queen_bitmasks();
    load_king_bitmasks();
}

