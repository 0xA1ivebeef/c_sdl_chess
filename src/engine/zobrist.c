
#include "engine/zobrist.h"

// init random int for every piece on every square (12*64)
// one number to indicate the side to move is black
// four number to indicate castling rights
// eight numbers to indicate the file of a valid en passant square if any
// 781 (12*64 + 1 + 4 + 8) random numbers

// TODO implement own pseudo random number generator

uint64_t get_zobrist_hash(Position* pos)
{
    return 0;
}
