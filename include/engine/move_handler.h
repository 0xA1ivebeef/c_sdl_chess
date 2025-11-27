
#ifndef MOVE_HANDLER_H
#define MOVE_HANDLER_H

#include <stdint.h>

#include "engine/game_context.h"
#include "engine/move.h"
#include "engine/castling.h"
#include "engine/double_pawn_push.h"

#include "utils/bitboard_utils.h"

void apply_move(uint64_t* bitboards, int startsquare, int destsquare);
int is_legal_move(GameContext* game, int startsquare, int destsquare);
int handle_move(GameContext* game, int startsquare, int destsquare);
int is_pawn_promotion(uint64_t* bitboards, int startsquare, int destsquare);
void handle_pawn_promotion(uint64_t* bitboards, int startsquare, int destsquare);

#endif
