
#include "include/legal_move_utils.h"

int get_endindex(move* legal_moves)
{
	int i = 0;
	while(legal_moves[i].startsquare != legal_moves[i].destsquare)
		++i;
	return i;
}

