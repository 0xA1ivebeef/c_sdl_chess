
#include "engine/special_move_handler.h"

// given move and bitboards, return if move is a pawn promotion
int is_pawn_promotion(uint64_t* bitboards, Move* this_move)
{
    int startsquare = this_move->startsquare;
    int destsquare = this_move->destsquare;

    int index = get_bitboard_index(bitboards, startsquare);
    if(!(index == BLACK_PAWN || index == WHITE_PAWN))
        return 0;

    return ((destsquare >= 0 && destsquare <= 7) || (destsquare >= 56 && destsquare <= 63));
}

// given bitboards and move, place queen for pawn
void handle_pawn_promotion(uint64_t* bitboards, Move* this_move)
{
    // TODO knight bishop rook 
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

int abs_int(int x) 
{
    return x < 0 ? -x : x;
}

// given bitboards and move, return if the move is a double pawn push
int is_double_pawn_push(Move* this_move, int ss_bb_i)
{
	printf("SPECIAL MOVE HANDLER double pawn push detected\n");
    int startsquare = this_move->startsquare;
    int destsquare = this_move->destsquare;
	
	printf("ss_bb_i = %d\n", ss_bb_i);
    if(!(ss_bb_i == BLACK_PAWN || ss_bb_i == WHITE_PAWN))
	{
		printf("moved piece is not a pawn\n");
        return 0;
	}

	printf("ss: %d, ds: %d\n", startsquare, destsquare);
    int res = (abs_int(startsquare - destsquare) == 16); // moved two squares 
    printf("SPEICAL MOVE HANDLER is double pawn push %d, %d: %s\n", this_move->startsquare, this_move->destsquare, res ? "TRUE" : "FALSE");
    return res;
}

// given move, set enpassant square in position (int)
void handle_double_pawn_push(int current_player, Move* this_move, int* enpassant_square)
{
    printf("HANDLING DOUBLE PAWN PUSH!\n");
    // wenn 0 dann -8, wenn 1 dann plus 8
    if(current_player == BLACK)
    {
        // black
        *enpassant_square = this_move->destsquare - 8;
        printf("can enpassant on square: %d\n", *enpassant_square);
    }
    else
    {
        // white
        *enpassant_square = this_move->destsquare + 8;
        printf("can enpassant on square: %d\n", *enpassant_square);
    }
}

// given bitboards, enpassant_square, apply capture enpassant 
void handle_enpassant(int current_player, uint64_t* bitboards, int enpassant_square)
{
    int i = !current_player * 6;
    int d = (enpassant_square + (16 * current_player - 8));
    printf("handling enpassant: game flag: %d, index = %d, square: %d\n", enpassant_square, i, d);
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

