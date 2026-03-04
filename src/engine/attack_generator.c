
#include "engine/attack_generator.h"

uint64_t get_pawn_attack_moves(int p, int sq, uint64_t* occ)
{
    uint64_t bm = p ? white_pawn_attack_bitmasks[sq] : black_pawn_attack_bitmasks[sq];
	return bm & occ[!p];
}

uint64_t get_knight_attack_moves(int p, int sq, uint64_t* occ)
{
    return (knight_bitmasks[sq] & ~occ[p]);
}

void atk_resolve_dir(int p, int sq, int dir, uint64_t* res, uint64_t* occ)
{
    int file = sq % 8;
    int rank = sq / 8;
    
    int x, y;
    int attacking_sq;

    for (int i = 1; i < 8; ++i)
    {
        x = file + i * file_offsets[dir];
        y = rank + i * rank_offsets[dir];

        if (x < 0 || x > 7 || y < 0 || y > 7)
            return;

        attacking_sq = y * 8 + x;

        if (occ[!p] & (1ULL << attacking_sq))
        {
            *res |= (1ULL << attacking_sq);
            return;
        }
        else if (occ[p] & (1ULL << attacking_sq))
            return;
        else
            *res |= (1ULL << attacking_sq);
    }
}

uint64_t get_sliding_piece_attack_moves(int p, int sq, int bb_index, uint64_t* occ)
{
    uint64_t res = 0;

    int startindex = (bb_index == 2 ? 4 : 0);
    int endindex = (bb_index == 3 ? 4 : 8);

    for (int dir = startindex; dir < endindex; ++dir)
        atk_resolve_dir(p, sq, dir, &res, occ);

    return res;
}

uint64_t get_king_attack_moves(int p, int sq, uint64_t* occ)
{
    return (king_bitmasks[sq] & ~occ[p]);
}

uint64_t get_attack_moves_bitmask(int p, int sq, int bb_index, uint64_t* occ)
{
	bb_index %= 6;
	switch(bb_index)
	{
		case 0:
			return get_pawn_attack_moves(p, sq, occ);
		case 1:
			return get_knight_attack_moves(p, sq, occ);
		case 2:
		case 3:
		case 4:
			return get_sliding_piece_attack_moves(p, sq, bb_index, occ);
		case 5:
			return get_king_attack_moves(p, sq, occ);
		default:
			return 0;
	}
}

uint64_t resolve_attack_bb(int p, uint64_t bb, int bb_index, uint64_t* occ)
{
	uint64_t res = 0;
	int sq = 0;
	while(bb)
	{
        if(bb & 1)
		    res |= get_attack_moves_bitmask(p, sq, bb_index, occ);
		bb >>= 1;
		++sq;
	}
	return res;
}

void attack_add_enpassant(uint64_t* res, uint8_t white, uint64_t pawn_bb, uint8_t enpassant)
{
    if (enpassant < 16 || enpassant > 47)
        return; 

    while (pawn_bb)
    {
        uint8_t sq = __builtin_ctzll(pawn_bb);
        pawn_bb &= pawn_bb - 1;

        uint64_t attacks = white 
            ? white_pawn_attack_bitmasks[sq] 
            : black_pawn_attack_bitmasks[sq];

        if (attacks & (1ULL << enpassant))
            *res |= (1ULL << enpassant);
    }
}

// TODO this includes uses enpassant maybe serarate that
uint64_t get_attack_bb(Position* pos, uint8_t player)
{
	uint64_t res = 0;
	int bb_startindex = player * 6;
	for(int i = bb_startindex; i < bb_startindex + 6; ++i)
        res |= resolve_attack_bb(player, pos->bb[i], i, pos->occ);

    // enpassant only possible for current player of position
    if (player == pos->player)
        attack_add_enpassant(&res, player, pos->bb[player*6], pos->enpassant);

    printf("attack_generator logging attack_bitboard\n");
    log_bitboard(&res);

    return res;
}

