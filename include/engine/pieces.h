
#ifndef PIECES_H
#define PIECES_H

typedef enum 
{ 
    BLACK, 
    WHITE
} Color;

typedef enum 
{
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING,
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    PIECE_COUNT
} Piece;

#endif
