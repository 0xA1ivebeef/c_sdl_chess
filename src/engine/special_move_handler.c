
#include "include/special_move_handler.h"

void handle_castling(uint64_t* bitboards, move* this_move, int* game_flags)
{
    int startsquare = this_move->startsquare;
    int destsquare = this_move->destsquare;
    int bb_index = (startsquare == 60) ? 9 : 3;
    if(startsquare > destsquare)
    {
        bitboards[bb_index] &= ~(1ULL << (destsquare - 2));
        bitboards[bb_index] |= (1ULL << (destsquare + 1));
    }
    else
    {
        bitboards[bb_index] &= ~(1ULL << (destsquare + 1));
        bitboards[bb_index] |= (1ULL << (destsquare - 1));
    }
    game_flags[1] &= (startsquare == 60) ? 3 : 12;
}

// set square behind pawn as capture in game flags
void handle_double_pawn_push(move* legal_move, int* game_flags)
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

