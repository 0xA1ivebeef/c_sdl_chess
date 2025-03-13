
#ifndef SPECIAL_MOVE_HANDLER_H
#define SPECIAL_MOVE_HANDLER_H

#include <stdio.h>
#include <stdint.h>
#include "move.h"

void handle_castling(uint64_t* bitboards, move* this_move, int* game_flags);
void handle_double_pawn_push(move* legal_move, int* game_flags);
void handle_enpassant(uint64_t* bitboards, int* game_flags);

#endif
