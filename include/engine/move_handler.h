
#ifndef MOVE_HANDLER_H
#define MOVE_HANDLER_H

#include <stdint.h>

#include "engine/game_context.h"
#include "engine/move.h"
#include "engine/castling.h"
#include "engine/double_pawn_push.h"

#include "utils/bitboard_utils.h"

void apply_move(uint64_t* bitboards, int startsquare, int destsquare);
int is_legal_move(uint64_t* bitboards, int startsquare, int destsquare, int* game_flags, Move* legal_moves);
int handle_move(uint64_t* bitboards, int startsquare, int destsquare, Move* legal_moves, int* game_flags);
int is_pawn_promotion(uint64_t* bitboards, int startsquare, int destsquare);
void handle_pawn_promotion(uint64_t* bitboards, int startsquare, int destsquare);

#endif
