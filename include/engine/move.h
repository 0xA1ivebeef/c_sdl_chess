
#ifndef MOVE_H
#define MOVE_H

#define NORMAL_FLAG       0
#define CASTLE_FLAG       1
#define ENPASSANT_FLAG    2
#define KNIGHT_PROMOTION  3
#define BISHOP_PROMOTION  4
#define ROOK_PROMOTION    5
#define QUEEN_PROMOTION   6

typedef struct
{
    int startsquare;
    int destsquare;
    int flags;
} Move;

#endif
