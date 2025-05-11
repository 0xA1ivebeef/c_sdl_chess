
#include "engine/double_pawn_push.h"

int is_double_pawn_push(uint64_t* bitboards, Move* this_move)
{
    int bb_index = get_bitboard_index(bitboards, this_move->startsquare);
    if(!(bb_index == 0 || bb_index == 6))
        return 0;
    return ((max(this_move->startsquare, this_move->destsquare) - min(this_move->startsquare, this_move->destsquare)) == 16);
}

// set square behind pawn as capture in game flags
void handle_double_pawn_push(Move* legal_move, int* game_flags)
{
    if(!game_flags[0])
    {
        // black
        game_flags[2] = legal_move->destsquare - 8;
        printf("can enpassant on square: %d\n", game_flags[2]);
    }
    else
    {
        // white
        game_flags[2] = legal_move->destsquare + 8;
        printf("can enpassant on square: %d\n", game_flags[2]);
    }
}

void handle_enpassant(uint64_t* bitboards, int* game_flags)
{
    // capture enpassanted pawn
    int p = game_flags[0];
    int index = !p*6;
    int d = (game_flags[2] + (16*p - 8));
    printf("handling enpassant: game flag: %d, index = %d, square: %d\n", game_flags[2], index, d);
    bitboards[index] &= ~(1ULL << d);
}

