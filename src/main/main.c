
#include "main/main.h"

int main()
{
    if (open_book("assets/Openings/Titans.bin") != 0)
        return -1;

    LegalMoves lm = {0};

    AppContext app = {0}; 
    if (init_sdl(&app) != 0) 
        return -1;

    load_bitmasks(); // external function for bitmask_loader.c
                         
    Position pos = {0}; 
    position_init(&pos, &lm);

    // running, selected_square, needs_update, game_over
    UIContext ui_context = { 1, INVALID_SQUARE, 0, 0 };
    render(&app, pos.bb);
    game_loop(&app, &pos, &ui_context, &lm); 
 
    cleanup(&app);
    return 0;
}

