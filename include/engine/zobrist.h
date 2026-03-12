
#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "engine/types.h"

extern const int WK_CASTLE;
extern const int WQ_CASTLE;
extern const int BK_CASTLE;
extern const int BQ_CASTLE; 

extern const int ENPASSANT_BASE;
extern const int WHITE_TO_MOVE;

extern const uint64_t Random64[781];

int open_book(const char* book_path);
uint8_t polyglot_sq(uint8_t sq);
int my_to_poly(int piece);
int poly_to_my(int poly);
uint64_t get_zobrist_hash(Position* pos);

#endif
