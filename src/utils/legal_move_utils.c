
#include "utils/legal_move_utils.h"

int get_legal_move_count(Move* legal_moves)
{
	int i = 0;
	while(legal_moves[i].start != legal_moves[i].dest)
		++i;
	return i;
}

