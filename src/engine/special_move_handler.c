
#include "engine/special_move_handler.h"

// TODO fix this
void handle_pawn_promotion(uint64_t* bb, Move* m)
{
    assert(m->flags >= KNIGHT_PROMOTION && m->flags <= QUEEN_PROMOTION);

    // move flags -> bb_i knight..queen 3..6
    int promote_bb_i = m->flags - 2; 

    int start_bb = (m->dest <= 7) ? 6 : 0;
    promote_bb_i += start_bb;

    int ds_i = get_bb_index(bb, m->dest);
    if (ds_i != -1)
        bb[ds_i] &= ~(1ULL << m->dest); // capture
                                                  
    // black n, b, r, q 7, 8, 9, 10
    bb[start_bb] &= ~(1ULL << m->start); // delete pawn
    bb[promote_bb_i] |= (1ULL << m->dest); // place queen
}

void handle_enpassant(int p, uint64_t* bb, uint8_t enpassant)
{
    int i = !p * 6;
    int d = (enpassant + 16*p - 8); // captured pawns square
    printf("handling enpassant: game flag: %d, index = %d, square: %d\n", 
            enpassant, i, d);
    bb[i] &= ~(1ULL << d); // capture enpassanted pawn
}

void handle_castling(uint64_t* bb, Move* m)
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
}

