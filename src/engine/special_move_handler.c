
#include "engine/special_move_handler.h"

// TODO fix this
void handle_pawn_promotion(uint64_t* bb, Move* m)
{
    // piece.h 
    int promote_bb = m->flags - 2; // knight..queen 3..6 

    int ds_i = get_bb_index(bb, m->dest);
    if (ds_i != -1)
        bb[ds_i] &= ~(1ULL << m->dest); // capture
                                                  
    int start_bb = 0;
    if(m->dest >= 0 && m->dest <= 7)
        start_bb = 6;

    promote_bb += start_bb;

    // black n, b, r, q 7, 8, 9, 10
    bb[start_bb] &= ~(1ULL << m->start); // delete pawn
    bb[promote_bb] |= (1ULL << m->dest); // place queen
}

// TODO fix this
void handle_enpassant(int p, uint64_t* bb, int enpassant)
{
    int i = !p * 6;
    int d = (enpassant + 16*p - 8); // captured pawns square
    printf("handling enpassant: game flag: %d, index = %d, square: %d\n", 
            enpassant, i, d);
    bb[i] &= ~(1ULL << d); // capture enpassanted pawn
}

// TODO: maybe dont set castle_rights here, just move the rook
// given bb and move and castle_rights, move the rook of the castling move and remove castle_rights
void handle_castling(uint64_t* bb, Move* m, int* castle_rights)
{
    int bbi = (m->start == 60) ? WHITE_ROOK : BLACK_ROOK; // rook bb
    if(m->start > m->dest)
    {
        bb[bbi] &= ~(1ULL << (m->dest - 2));
        bb[bbi] |= (1ULL << (m->dest + 1));
    }
    else
    {
        bb[bbi] &= ~(1ULL << (m->dest + 1));
        bb[bbi] |= (1ULL << (m->dest - 1));
    }

    // TODO this might cause bugs (take away castlerights)
    *castle_rights &= (m->start == 60) ? 3 : 12; 
}

