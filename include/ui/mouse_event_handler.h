
#ifndef MOUSE_EVENT_HANDLER_H
#define MOUSE_EVENT_HANDLER_H

#include <stdint.h>

#include "engine/constants.h"

int handle_mouse_event(int mouse_x, int mouse_y, uint64_t occupancy_bitboard);

#endif
