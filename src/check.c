
#include "include/check.h"

int is_check(int p, uint64_t* bitboards, uint64_t attack_bitboard)
{
	uint64_t king_bitboard = bitboards[6*p + 5];
	if(attack_bitboard & king_bitboard)
		return 1;
	return 0;
}

