
#include "engine/special_move_handler.h"

// given move and bitboards, return if move is a pawn promotion
int is_pawn_promotion(uint64_t* bitboards, Move* this_move)
{
    int startsquare = this_move->startsquare;
    int destsquare = this_move->destsquare;

    int index = get_bitboard_index(bitboards, startsquare);
    if(!(index == WHITE_PAWN || index == BLACK_PAWN))
        return 0;
    return ((destsquare >= 0 && destsquare <= 7) || (destsquare >= 56 && destsquare <= 63));
}

// given bitboards and move, place queen for pawn
void handle_pawn_promotion(uint64_t* bitboards, Move* this_move)
{
    int startsquare = this_move->startsquare;
    int destsquare = this_move->destsquare;

    int ds_i = get_bitboard_index(bitboards, destsquare);
    bitboards[ds_i] &= ~(1ULL << destsquare);
    if(destsquare >= 0 && destsquare <= 7)
    {
        bitboards[6] &= ~(1ULL << startsquare); // delete pawn
        bitboards[10] |= (1ULL << destsquare); // place queen
    }   
    else
    { 
        bitboards[0] &= ~(1ULL << startsquare); // delete pawn
        bitboards[4] |= (1ULL << destsquare); // place queen
    }
}

// given bitboards and move, return if the move is a double pawn push
int is_double_pawn_push(uint64_t* bitboards, Move* this_move)
{
    int startsquare = this_move->startsquare;
    int destsquare = this_move->destsquare;

    int bb_index = get_bitboard_index(bitboards, startsquare);
    if(!(bb_index == 0 || bb_index == 6))
        return 0;
    return ((max(startsquare, destsquare) - min(startsquare, destsquare)) == 16);
}

// given move, set game flags[2] as enpassant square (int)
void handle_double_pawn_push(Move* this_move, int* game_flags)
{
    // wenn 0 dann -8, wenn 1 dann plus 8
    if(!game_flags[0])
    {
        // black
        game_flags[2] = this_move->destsquare - 8;
        printf("can enpassant on square: %d\n", game_flags[2]);
    }
    else
    {
        // white
        game_flags[2] = this_move->destsquare + 8;
        printf("can enpassant on square: %d\n", game_flags[2]);
    }
}

// given bitboards and game flags, apply the capture en passant 
void handle_enpassant(uint64_t* bitboards, int* game_flags)
{
    int p = game_flags[0];
    int index = !p*6;
    int d = (game_flags[2] + (16*p - 8));
    printf("handling enpassant: game flag: %d, index = %d, square: %d\n", game_flags[2], index, d);
    bitboards[index] &= ~(1ULL << d); // capture enpassanted pawn
}

// given bitboards and move and game flags, move the rook of the castling move 
void handle_castling(uint64_t* bitboards, Move* this_move, int* game_flags)
{
    int startsquare = this_move->startsquare;
    int destsquare = this_move->destsquare;
    int bb_index = (startsquare == 60) ? 9 : 3; // rook bitboards
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


