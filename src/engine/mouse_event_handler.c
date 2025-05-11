
#include "include/mouse_event_handler.h"

int handle_mouse_event(int mouse_x, int mouse_y, uint64_t occupancy_bitboard)
{
    int square = mouse_y / TILESIZE * 8 + mouse_x / TILESIZE;
    if(occupancy_bitboard & (1ULL << square))
        return -1;
    return square;
}


