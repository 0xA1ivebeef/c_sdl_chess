
#include "main/main.h"

int main()
{
    LegalMoves legal_moves = {0};
    init_zobrist();

    AppContext app = {0}; 
    if (init_sdl(&app) != 0) 
        return -1;

    load_bitmasks(); // external function for bitmask_loader.c
                         
    Position pos = {0}; 
    position_init(&pos, &legal_moves);

    // running, selected_square, needs_update, game_over
    UIContext ui_context = { 1, INVALID_SQUARE, 0, 0 };
    render(&app, pos.bb);
    game_loop(&app, &pos, &ui_context, &legal_moves); 
 
    cleanup(&app);
    return 0;
}

