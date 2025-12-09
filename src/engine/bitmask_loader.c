
#include "engine/bitmask_loader.h"

uint64_t bitmasks[11][64] = {0};

const int knight_file_offsets[8] = { 2, 2, 1, -1, -2, -2, -1,  1 };
const int knight_rank_offsets[8] = { -1, 1, 2,  2,  1, -1, -2, -2 };

int is_on_board(int file, int rank)
{
	return (file >= 0 && file < 8 && rank >= 0 && rank < 8);
}

void load_white_pawn_normal_bitmasks(uint64_t* wpn)
{
    int file, rank;
    for (int i = 8; i < 56; ++i)
    {
	    file = i % 8;
	    rank = i / 8;

	    wpn[i] |= (1ULL << ((rank - 1) * 8 + file));
    }   
}

void load_black_pawn_normal_bitmasks(uint64_t* bpn)
{
    int file, rank;
    for (int i = 8; i < 56; ++i)
    {
	    file = i % 8;
	    rank = i / 8;

	    bpn[i] |= (1ULL << ((rank + 1) * 8 + file));
    }
}

void load_white_pawn_double_bitmasks(uint64_t* wpd)
{
    for (int i = 48; i < 56; ++i)
	    wpd[i] |= (1ULL << (i - 16));
}

void load_black_pawn_double_bitmasks(uint64_t* bpd)
{
    for (int i = 8; i < 16; ++i)
	    bpd[i] |= (1ULL << (i + 16));
}

void load_white_pawn_capture_bitmasks(uint64_t* wpc)
{
    int file, rank;
    for (int i = 8; i < 56; ++i)
    {
	    file = i % 8;
	    rank = i / 8;

	    if (file - 1 >= 0)
		    wpc[i] |= (1ULL << ((rank - 1) * 8 + file - 1));

	    if (file + 1 < 8)
		    wpc[i] |= (1ULL << ((rank - 1) * 8 + file + 1));
    }
}

void load_black_pawn_capture_bitmasks(uint64_t* bpc)
{
    int file, rank;
    for (int i = 8; i < 56; ++i)
    {
	    file = i % 8;
	    rank = i / 8;

	    if (file - 1 >= 0)
		    bpc[i] |= (1ULL << ((rank + 1) * 8 + file - 1));

	    if (file + 1 < 8)
		    bpc[i] |= (1ULL << ((rank + 1) * 8 + file + 1));
    }
}

void load_knight_bitmasks(uint64_t* n)
{
    int file, rank;
    int this_file, this_rank;
    for (int i = 0; i < 64; ++i)
    {
	    file = i % 8;
	    rank = i / 8;
	    for (int dir = 0; dir < 8; ++dir)
	    {
		    this_file = file + knight_file_offsets[dir];
		    this_rank = rank + knight_rank_offsets[dir];

		    if (!is_on_board(this_file, this_rank))
			    continue;

		    n[i] |= (1ULL << (this_rank * 8 + this_file));
	    }
    }
}

void load_bishop_bitmasks(uint64_t* b)
{
    int file, rank;
    int this_file, this_rank;
    for (int i = 0; i < 64; ++i)
    {
	    file = i % 8;
	    rank = i / 8;
	    for (int dir = 4; dir < 8; ++dir)
	    {
		    for (int j = 1; j < 8; ++j)
		    {
			    this_file = file + j * file_offsets[dir];
			    this_rank = rank + j * rank_offsets[dir];

			    if (!is_on_board(this_file, this_rank))
				    continue;

			    b[i] |= (1ULL << (this_rank * 8 + this_file));
		    }
	    }
    }
}

void load_rook_bitmasks(uint64_t* r)
{
    int this_file, this_rank;
    for (int i = 0; i < 64; ++i)
    {
	    this_file = i % 8;
	    this_rank = i / 8;
	    for (int j = 0; j < 8; ++j)
	    {
		    r[i] |= (1ULL << (this_rank * 8 + j));
		    r[i] |= (1ULL << (this_file + j * 8));
	    }
	    r[i] &= ~(1ULL << i);
    }
}

void load_queen_bitmasks(uint64_t* b, uint64_t* r, uint64_t* q)
{
    for (int i = 0; i < 64; ++i)
	    q[i] = b[i] | r[i];
}

void load_king_bitmasks(uint64_t* k)
{
    int file, rank;
    int this_file, this_rank;
    for (int i = 0; i < 64; ++i)
    {
	    file = i % 8;
	    rank = i / 8;
	    for (int dir = 0; dir < 8; ++dir)
	    {
		    this_file = file + file_offsets[dir];
		    this_rank = rank + rank_offsets[dir];

		    if (!is_on_board(this_file, this_rank))
			    continue;

		    k[i] |= (1ULL << (this_rank * 8 + this_file));
	    }
    }
}

void load_bitmasks()
{
    load_white_pawn_normal_bitmasks(bitmasks[0]);
    load_black_pawn_normal_bitmasks(bitmasks[1]);
    load_white_pawn_double_bitmasks(bitmasks[2]);
    load_black_pawn_double_bitmasks(bitmasks[3]);
    load_white_pawn_capture_bitmasks(bitmasks[4]);
    load_black_pawn_capture_bitmasks(bitmasks[5]);
    load_knight_bitmasks(bitmasks[6]);
    load_bishop_bitmasks(bitmasks[7]);
    load_rook_bitmasks(bitmasks[8]);
    load_queen_bitmasks(bitmasks[7], bitmasks[8], bitmasks[9]);
    load_king_bitmasks(bitmasks[10]);
}
