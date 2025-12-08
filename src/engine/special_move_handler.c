
#include "engine/special_move_handler.h"

// given bitboards and move, place queen for pawn
void handle_pawn_promotion(uint64_t* bitboards, Move* this_move)
{
    int startsquare = this_move->startsquare;
    int destsquare = this_move->destsquare;

    // piece.h 
    int promote_bb = this_move->flags - 2; // knight..queen 3..6 

    int ds_i = get_bitboard_index(bitboards, destsquare);
    if (ds_i != -1)
        bitboards[ds_i] &= ~(1ULL << destsquare); // capture
                                                  
    int start_bb = 0;
    if(destsquare >= 0 && destsquare <= 7)
        start_bb = 6;

    promote_bb += start_bb;

    // black n, b, r, q 7, 8, 9, 10
    bitboards[start_bb] &= ~(1ULL << startsquare); // delete pawn
    bitboards[promote_bb] |= (1ULL << destsquare); // place queen`
}

// given move, set enpassant square in position (int)
void handle_double_pawn_push(int current_player, Move* this_move, int* enpassant_square)
{
    // wenn 0 dann -8, wenn 1 dann plus 8
    if(current_player == BLACK)
        *enpassant_square = this_move->destsquare - 8;
    else
        *enpassant_square = this_move->destsquare + 8;
}

// given bitboards, enpassant_square, apply capture enpassant 
void handle_enpassant(int current_player, uint64_t* bitboards, int enpassant_square)
{
    int i = !current_player * 6;
    int d = (enpassant_square + (16 * current_player - 8));
    // printf("handling enpassant: game flag: %d, index = %d, square: %d\n", enpassant_square, i, d);
    bitboards[i] &= ~(1ULL << d); // capture enpassanted pawn
}

// TODO: maybe dont set castle_rights here, just move the rook
// given bitboards and move and castle_rights, move the rook of the castling move and remove castle_rights
void handle_castling(uint64_t* bitboards, Move* this_move, int* castle_rights)
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
    *castle_rights &= (startsquare == 60) ? 3 : 12; // take away castlerights
}

